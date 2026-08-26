"""Derive REW-0031 normal and roughness PNG sources from the retained kit JPEGs.

Run with the workstation Python (Pillow 11+):
    py -3 Rewind/ArtSource/Scripts/generate_surface_detail_maps.py

The output remains below ArtSource, rather than Content, so the imported assets
can be reproduced from project-owned source material.  Maps retain the 1254 px
source resolution (well below the task's 2048 px limit).  The height field is
luminance plus material-tuned local contrast; Sobel gradients use wrapped
neighbours so the normal detail tiles with the source image.
"""

from __future__ import annotations

from pathlib import Path
from PIL import Image, ImageFilter, ImageOps


ART_SOURCE_DIR = Path(__file__).resolve().parents[1]
SOURCE_DIR = ART_SOURCE_DIR / "Textures" / "EnvironmentTextureKit-v1"
OUTPUT_DIR = SOURCE_DIR / "Derived_REW-0031"

# normal_strength, roughness centre (the MI scalar remains the material's
# overall control), roughness variation, darker-pixel influence.
SURFACES = {
    "BrickDark_Aged": (3.0, 0.93, 0.22, 0.14),
    "CeramicTile_Ivory_Aged": (1.2, 0.82, 0.16, 0.10),
    "FloorTile_Interior": (1.8, 0.90, 0.18, 0.12),
    "MetalBlack_Painted_Aged": (1.6, 0.84, 0.24, 0.20),
    "PavingStone_Courtyard": (3.2, 0.96, 0.25, 0.13),
    "PlasterGray_Aged": (2.2, 0.96, 0.18, 0.10),
    "PlasterGreen_Aged": (2.0, 0.94, 0.19, 0.12),
    "WoodDark_Worn": (2.6, 0.88, 0.22, 0.15),
}


def clamp_byte(value: float) -> int:
    return max(0, min(255, round(value)))


def wrapped_sobel(height: list[int], width: int, height_px: int, strength: float) -> Image.Image:
    """Build an Unreal-compatible tangent-space RGB normal from a height field."""
    normal = bytearray(width * height_px * 3)
    for y in range(height_px):
        up = ((y - 1) % height_px) * width
        here = y * width
        down = ((y + 1) % height_px) * width
        for x in range(width):
            left = (x - 1) % width
            right = (x + 1) % width
            dx = (height[up + right] + 2 * height[here + right] + height[down + right]
                  - height[up + left] - 2 * height[here + left] - height[down + left])
            dy = (height[down + left] + 2 * height[down + x] + height[down + right]
                  - height[up + left] - 2 * height[up + x] - height[up + right])
            nx = -dx * strength / 1020.0
            ny = -dy * strength / 1020.0
            nz = 1.0
            length = (nx * nx + ny * ny + nz * nz) ** 0.5
            index = (here + x) * 3
            normal[index] = clamp_byte((nx / length * 0.5 + 0.5) * 255)
            # UE's normal-map convention has +Y downward in texture space.
            normal[index + 1] = clamp_byte((ny / length * 0.5 + 0.5) * 255)
            normal[index + 2] = clamp_byte((nz / length * 0.5 + 0.5) * 255)
    return Image.frombytes("RGB", (width, height_px), bytes(normal))


def build_surface(name: str, settings: tuple[float, float, float, float]) -> None:
    normal_strength, roughness_centre, roughness_variation, dark_influence = settings
    source = SOURCE_DIR / f"T_REW_Surface_{name}_BC.jpg"
    with Image.open(source) as loaded:
        colour = loaded.convert("RGB")
    luminance = ImageOps.grayscale(colour)
    # Detail relative to a soft local baseline makes grout, wear and grain read
    # without treating the overall source brightness as a giant height ramp.
    baseline = luminance.filter(ImageFilter.GaussianBlur(radius=6))
    lum = list(luminance.getdata())
    base = list(baseline.getdata())
    height = [clamp_byte(128 + (value - local) * 2.2 + (value - 128) * 0.20)
              for value, local in zip(lum, base)]

    normal = wrapped_sobel(height, luminance.width, luminance.height, normal_strength)
    roughness = bytearray(luminance.width * luminance.height)
    for index, (value, local) in enumerate(zip(lum, base)):
        detail = abs(value - local) / 127.5
        darkness = (255 - value) / 255.0
        value01 = roughness_centre + detail * roughness_variation + darkness * dark_influence
        roughness[index] = clamp_byte(value01 * 255)

    normal.save(OUTPUT_DIR / f"T_REW_Surface_{name}_N.png", optimize=True)
    Image.frombytes("L", luminance.size, bytes(roughness)).save(
        OUTPUT_DIR / f"T_REW_Surface_{name}_R.png", optimize=True
    )
    print(f"generated {name}: {luminance.width}x{luminance.height}")


def main() -> None:
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    for name, settings in SURFACES.items():
        build_surface(name, settings)


if __name__ == "__main__":
    main()
