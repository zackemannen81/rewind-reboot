#pragma once

#include "CoreMinimal.h"

/**
 * Identifiers for player-facing copy. Actors speak by id. The catalog owns the
 * line. `docs/design/player-messages.md` owns the channel.
 */
namespace RewindMessageIds
{
	const FName CharacterControls{TEXT("Character.Controls")};

	const FName LockPrompt{TEXT("Lock.Prompt")};
	const FName LockCodeBuffer{TEXT("Lock.CodeBuffer")};
	const FName LockAccepted{TEXT("Lock.Accepted")};
	const FName LockRejected{TEXT("Lock.Rejected")};

	const FName FuseCarried{TEXT("Fuse.Carried")};
	const FName FuseSeatedBuilding{TEXT("Fuse.SeatedBuilding")};
	const FName FuseSeatedCourtyard{TEXT("Fuse.SeatedCourtyard")};
	const FName FuseTakenBuilding{TEXT("Fuse.TakenBuilding")};
	const FName FuseTakenCourtyard{TEXT("Fuse.TakenCourtyard")};

	const FName SocketEmptyOther{TEXT("Socket.EmptyOther")};
	const FName SocketEmptyAtRest{TEXT("Socket.EmptyAtRest")};

	const FName GeneratorDead{TEXT("Generator.Dead")};
	const FName GeneratorOnline{TEXT("Generator.Online")};

	const FName LiftNoPower{TEXT("Lift.NoPower")};

	const FName AnchorAccepted{TEXT("Anchor.Accepted")};
	const FName AnchorRefused{TEXT("Anchor.Refused")};

	const FName RadioChannelStatic{TEXT("Radio.ChannelStatic")};
	const FName RadioChannelVoice{TEXT("Radio.ChannelVoice")};
	const FName RadioDigitSeven{TEXT("Radio.DigitSeven")};
	const FName RadioDigitThree{TEXT("Radio.DigitThree")};
	const FName RadioDigitOne{TEXT("Radio.DigitOne")};
	const FName RadioDigitTwo{TEXT("Radio.DigitTwo")};
	const FName RadioCodeObtained{TEXT("Radio.CodeObtained")};
}
