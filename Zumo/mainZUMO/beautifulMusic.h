/*
Composed by the one and only Espen van Bothooven
This files includes music and sounds used by the car
*/

#include <Arduino.h>

const char BlueDanube [] PROGMEM =  //Beautiful
  "!T84 L8 ddf#aa4 O5 aa4f#f#4"
  " O4 ddf#aa4 O5 aa4gg4"
  " O4 c#c#ebb4 O5 bb4gg4"
  " O4 c#c#ebb4 O5 bb4f#f#4"
  " O4 ddf#a>d4 O6 dd4<a<a4"
  " O4 ddf#a>d4 O6 dd4<b<b4"
  " O4 eegbb2g#a  >f#2df#f#4eb4ad4dd4.";

const char ImperialMarch [] PROGMEM = //<3
  "!T100 O3 aaaf8.>c16af8.>c16a2"
  " O4 eeef8.c16 O3 a-f8.>c16a2"
  " O4 a<aa L16 a-8.gg-fg-"
  " L4 R<b-8e- L16 d8.d-c<bc"
  " L4 Rf8a-f8.>c16a f8.>c16a1";

// "A beautiful piece of music, for an enormously disapointing series"
const char GoT [] PROGMEM =
  "!T87 L8 gce-16f16 gce-16f16 gce-16f16"
  " gce16f16gce16f16gce16f16gce16f16g4."
  " L4 g.c.e-16f16gce-16f16"
  " L8 d<g<b-16c16 d<g<b-16c16 d<g<b-16c16d4."
  " L4 f.<b-.e-16d16fb-e-16d16"
  " O3 L8 >cfa-16b-16 >cfa-16b-16 >cfa-16b-16>c4.";

const char Morgenstemning[] PROGMEM =
  "!T60 L8 gedcdegedcd16e16d16e16gegaeagedc2";

const char StarWars [] PROGMEM =
  "!T112 L16 d8.dg2>d2>cba"
  "O5 g2rd4c<b<ag2rd4c<bc<a1";

const char Prelude [] PROGMEM =
  "!T130 L16 O5 cgfge-gfgcgfge-gfg"
  " ca-ga-fa-ga-ca-ga-fa-ga-ca-ga-fa-ga-ca-ga-fa-ga-"
  " ce-de-ge-de-ce-de-ge-de-"
  " c>cb->ca->cb->c c>cb->ca->cb->c"
  " cagag-aga cagag-aga"
  " <b-b-ab-gb-ab- <b-b-ab-gb-ab-"
  " <b-gfgegfg <b-gfgegfg"
  " <a-a-ga-fa-ga- <a-a-ga-fa-ga-"
  " <a-dcdfdcd <a-dcdfdcd"
  " <ge-de-ge-de- <ge-de-ge-de-"
  " ce-de-a-e-de- ce-de-a-e-de-"
  " dfe-fa-fe-f dfe-fa-fe-f"
  " e-gfga-gfg e-gfga-gfg"
  " e-agafaga e-agafaga"
  " dfe-fa-fe-f dfe-fa-fe-f"
  " cfefa-fef cfefa-fef"
  " ce-de-fe-de- ce-de-fe-de-"
  " <a-e-de-ce-de- <a-e-de-ce-de-"
  " <ge-de-fe-de- <ge-de-fe-de-"
  " <ge-de-ce-de- <ge-de-ce-de-"
  " <ge-de-fe-de- <ge-de-fe-de- <ge-de-fe-de- <ge-de-fe-de-"
  " O4 <g<bdfa-fefbf>dba-fef"
  " <gce-g>cgf+g O5 e-cge-c<g<f+"
  " O4 <gaf+ O5 ce-c<bcgcag-e-c<bc"
  " O3 g2a-2f2g2";

const char CrazyTrain [] PROGMEM =
  "!T124 O3 L8 f#f#>c#>df#>c#>c#"
  " bag#abag#e"
  " f#f#>c#>df#>c#>c#"
  " bag#abag#e"
  " f#f#>c#>df#>c#>c#"
  " bag#abag#e"
  " f#f#>c#>df#>c#>c#>d2>e2";

const char Dragnet[] PROGMEM =
  "!T80 L8 c.d16e-16r16cR2c.d16e-16r16cg-2";

const char Rosanna [] PROGMEM =
  "!T166 O3 L8 >e->e-4 b-b-4 ff4"
  " O4 L16 <g#<acdf8g#gfd8fr8"
  " T83 O5 L8 cdde-16e-16r8de-16e-16r8.r8"
  " O3 L16 >e->e-8.b-b-8.ff8.r8"
  " O5 L8 cdde-16e-16r8de-16e-16r8.r8"
  " L16 e-e-8.dd8.cc8.r8"
  " L8 cdde-16e-16r8de-16e-16r8.r8"
  " O3 L16 >e->e-8.b-b-8.ff8.r8"
  " O5 L8 cdde-16e-16r8de-16e-16r8.r8"
  " O4 L16 e-e-8.<b-<b-8.ff8.r8";
