# RE:WIND — Docs-First Bootstrap and Recovery Plan

## Utgångspunkt

RE:WIND startas om som ett nytt docs-first-projekt.

Det befintliga RE:WIND-materialet behandlas som **source material och historisk design**, inte som automatisk authority. Ingen gammal GDD, roadmap, task-lista eller implementation får antas beskriva aktuell sanning förrän den har klassificerats och uttryckligen återförts till en ägande docs-first-yta.

Den centrala spelhypotesen behålls:

> RE:WIND är ett tredjepersons psykologiskt sci-fi/pusselspel där världen återställs i deterministiska loopar medan spelarens kunskap består. Spelaren blir mäktigare genom att förstå världen, inte genom traditionell progression.

Detta är produktens riktning. Exakta mekaniker blir authority först när de beslutas genom projektets task- och decision-flöde.

## Docs-first är projektets styrsystem

Projektet börjar med den strukturerade docs-first-basen.

```text
AGENTS.md
docs/
├── CURRENT_TASK.md
├── TASK_WORKFLOW.md
├── TASK_IDS.md
├── PROJECT_BRIEF.md
├── CURRENT_STATUS.md
├── SYSTEMDOC.md
├── FILESTRUCTURE.md
├── CONTRIBUTING.md
├── JOURNAL.md
├── design/
├── adr/
├── acceptance/
├── backlog/
├── paused/
├── finished/
└── concepts_sandbox/
```

Ingen separat RE:WIND-process införs bredvid detta.

RE:WIND använder docs-first-processen som projektets enda operativa arbetsmodell.

## Legacy-material

Gamla GDD:n, roadmapen, tidigare designförslag och den befintliga analysen importeras som icke-auktoritativt source material.

Exempel:

```text
docs/concepts_sandbox/legacy-rewind/
├── README.md
├── original-gdd/
├── old-roadmaps/
├── old-task-material/
└── recovery-analysis.md
```

`README.md` ska uttryckligen säga att materialet:

* beskriver tidigare intentioner och experiment;
* kan innehålla motsägelser;
* inte beskriver nuvarande implementation;
* inte får implementeras direkt;
* endast kan få authority genom explicit restatement i ett ägande dokument via en aktiverad task eller ett beslut.

Det gamla GDD:t arkiveras alltså inte som gammal authority som fortfarande måste tolkas. Det blir input till ny canonicalization.

## Första tasken — etablera sanningen

Första tasken ska **inte bygga Chapter 1**.

Den ska etablera ett sanningsenligt RE:WIND-repo.

Exempel:

```text
REW-0001 — Establish canonical project state
```

### Goal

Gör det möjligt för en ny människa eller agent att förstå vad RE:WIND är, vad som faktiskt finns idag och vilken minsta spelhypotes som ska bevisas först.

### In scope

* fastställ projektets produktvision;
* inventera den befintliga implementationen;
* skilj faktisk implementation från gamla DONE-claims;
* definiera projektets nuvarande tekniska bas;
* etablera repository layout;
* identifiera konflikter i legacy-designen;
* skapa förslag till nästa bounded task.

### Out of scope

* ny gameplay-implementation;
* environment polish;
* nya assets;
* Chapter 2–5;
* generell tidsmotor;
* procedural asset pipeline;
* implementation av gamla GDD-idéer enbart därför att de finns dokumenterade.

## PROJECT_BRIEF — äger produktens riktning

`PROJECT_BRIEF.md` ska beskriva den beslutade produkten på hög nivå.

Den ska exempelvis äga:

* RE:WIND:s kärnfantasi;
* målspelare;
* produktmål;
* grundprincipen `world is deterministic / player accumulates knowledge`;
* Unity som vald motor tills ett separat beslut ändrar detta;
* att första produktbeviset är Chapter 1 / Five Loops Test;
* explicita non-goals för den aktuella fasen.

Den ska inte innehålla detaljerade implementationer av Echo, Anchors eller senare chapters.

## CURRENT_STATUS — äger verkligheten nu

Den gamla implementationen granskas utan hänsyn till tidigare task-status.

Exempel på sanningsenlig status:

```text
TimeManager
- exists
- loop timer exists
- canonical world reset not implemented

KnowledgeManager
- basic persistence exists
- currently implemented through PlayerPrefs

AnchorManager
- active anchor identifiers can be stored
- canonical world-state overrides are not implemented

EchoSystem
- transform sampling exists
- deterministic EchoAction replay is not implemented
```

Tidigare `DONE` betyder inte att funktionen är färdig om implementationen inte stöder claimen.

Detta är särskilt viktigt eftersom den gamla dokumentationen anger flera kärnsystem som färdiga trots att world reset, canonical anchor application och riktigt Echo-handoff fortfarande saknas.

## Design authority

Ny design skapas endast när en task behöver den.

I stället för ett gigantiskt GDD kan authority delas efter faktisk semantik:

```text
docs/design/
├── game-rules.md
├── world-state-model.md
├── chapter-1.md
└── vertical-slice-vs0.md
```

Dokumenten skapas inte på förhand bara för att strukturen ser komplett ut.

Varje dokument ska ha ett tydligt ownershipområde och inga två dokument får definiera samma regel.

### Exempel

`game-rules.md` kan äga:

* determinism;
* loop semantics;
* knowledge persistence;
* vad Anchor betyder;
* vilka regler Echo följer.

`world-state-model.md` kan äga:

* BaselineState;
* LoopState;
* KnowledgeState;
* AnchorState;
* EchoRecord;
* StoryState;
* reset/apply-order.

`chapter-1.md` äger endast authored Chapter 1-innehåll.

`vertical-slice-vs0.md` äger det testbara produktbeviset.

## Beslut som måste tas explicit

Konflikter i legacy-materialet löses inte genom att någon “väljer den rimligaste versionen”.

De blir beslut.

Exempel:

```text
ADR — World determinism

Decision:
The world does not learn the player between normal loops.
The player learns the world.

Consequence:
Any future case where the world remembers the player is an explicit
narrative rule violation, not baseline AI behavior.
```

Samma sak gäller bland annat:

* Unity kontra Unreal;
* Anchor-semantik;
* Echo-interaktion;
* Insight;
* senare temporal mechanics.

## Concepts sandbox

Allt intressant som ännu inte är beslutat stannar här.

Exempel:

```text
docs/concepts_sandbox/
├── future-temporal-mechanics/
│   ├── backwards-time.md
│   ├── parallel-loops.md
│   └── future-causality.md
├── narrative-alternatives/
└── environment-pipeline-ideas/
```

En agent får aldrig implementera något härifrån enbart därför att det verkar passa spelet.

Promotion sker endast:

```text
concept
↓
activated task / discovery
↓
explicit decision
↓
restatement in owning authority
↓
implementation
```

## Nästa task — VS-0

När REW-0001 är färdig och arkiverad kan nästa task charteras.

```text
REW-0002 — Five Loops Test
```

Detta blir första riktiga implementationstasken.

Målet är inte “build Chapter 1”.

Målet är att bevisa produktens kärnhypotes:

> Är det roligt att uppleva samma deterministiska värld flera gånger och bli objektivt bättre därför att man vet mer?

Acceptance ska vara konkret.

Från en ren save ska spelaren kunna:

1. vakna i 4C;
2. spela en riktig loop;
3. se världen återställas deterministiskt;
4. upptäcka `7312`;
5. behålla kunskapen efter reset;
6. se fuse/generator/gate återställas korrekt;
7. observera samma reproducerbara patrol;
8. lära sig och utnyttja patrolmönstret;
9. skapa den första explicita Anchor-override:n;
10. starta nästa loop med gårdsgrinden redan stabiliserad;
11. nå Transit Hub;
12. genomföra samma sekvens märkbart snabbare när lösningen redan är känd.

Detta motsvarar den minsta hypotes som den befintliga designen redan stödjer.

## Scope pressure

Allt som upptäcks under REW-0002 routas genom `TASK_WORKFLOW.md`.

Exempel:

```text
"Echo behöver action replay för den här tasken"
→ om blockerande: bounded child task

"Det vore coolt med local-time bubbles"
→ concepts_sandbox eller backlog beroende på beslutad project scope

"Vi behöver bättre väggmaterial"
→ backlog om det inte behövs för acceptance

"World reset fungerar inte"
→ inne i tasken om det är del av charterad deliverable
```

Ingen upptäckt får automatiskt expandera chartern.

## Definition av första framgång

RE:WIND:s första framgång är inte:

* snygg Apartment 4C;
* komplett GDD;
* fem chapters;
* procedural asset generation;
* avancerad temporal architecture.

Första framgången är:

```text
cold repository
↓
known authority
↓
bounded task
↓
working deterministic loop
↓
persistent knowledge
↓
first anchor
↓
measurably faster learned run
↓
verified and archived
```

När detta fungerar har projektet både klarat sitt första docs-first-arbetsflöde och bevisat RE:WIND:s grundläggande speldesign.
