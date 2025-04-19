read production timeline on reduced scope
	read each game system file for tasks
	^ move from here to there, and there to here
	when in doubt, work on whatever

# aspects/chunks:
+ performance + new feature
- code (each day a bit, related to each aspect)
- art (on weekends)
* architecture
* interactions
* boss
* story (dialogs)
* sound
+ build & publish
+ feedbackations (and feedback)

# general
* publish ost on everywhere.

* the ones from  spirit

## for build
* build demo
* g send email saying that a new build has been uploaded (test first)

## ongoing/refactor:
* continue fixing nsloctext and ftext::fromstring
* add item range to enums ENUM_RANGE_BY_COUNT(EItemUseResult, EItemUseResult::MAX);
* use new room datalayers
* rename objects / ensure labels
* use hints, autodialogs, conditions
** on level. ensure to reset the interact collision to none
** radio, batts, heater, calendar, clouts

## fix
* fix the game start
* * fix: the camera fade
* fix: item.use is added after spot.trigger
* fix feedback form opens twice. move to another place
* fix: rename G_FLY_CAM to DBG_FLY_CAM ( requires to fix char delegate binding)
* * this is also broken and does not work.
* fix: rethink books01 animator with component instead of subsystem

## new

* card what works turns items into usable ones.
  ** merges, unlocks, reduce cool downs, etc. shows dialog
  *** add autodialog with items as params

* animator to change owners dilation

* sentry
  *** trigger calls on another thread? maybe not, could make report errors harder. sentry is for errors only so...
** on lbutton add breadcrumb on click with name
*** might need to create an lbutton

* new: on savegame slot on intro menu, show also the played time (get from the flags)

ch0
* arch: clouts on wardrobe room 2
* new: maybe: improve the count time so that it's not affected by time dilation, or not? (maybe won't fix, so that it does not count the time in settings, but maybe it should?)
* new: add stuff to the status overlay ui
*  idea: add to eval
* operator ',' that appends to a list. if the left is empty it starts a list.
* operator ; chooses an item from a list based on the index

* eval function 'pick': returns a random element from a list
# architecture

living00
hall00
room 2
* arch: clouts on wardrobe room 2
room 3

kitchen
* decoration? on top and inside the cabs

street
* add pavement

# interactions
## chap every
* poem, card, doors, word cubes (remember shadows), tape, batteries, hints.
* shadows on objects that needs it: need to find which ones.

## general
* remove use of cards from interacts. it makes them confusing.
* * remove from door on ch02 and character on ch01.
* rework puzzles to take advantage of the rework. E.g. the piano can use the mesh on the puzzle instead of another interact trigger.
* add light switches to the scenes to control wall lights
* stuff with phone puzzle
* idea: toggling light a few times "breaks" it, and it doesn't work anymore (lock/active)

chap04 integrate-love-accept
* add word puzzle?
* make one of the boxes into an interact with some stale remaining and complain (maybe w/cockroach)
* idea: inter/story: ch04: need to grab some cutlery in addition to the plate.

chap03 fawn-shame-persevere

chap02 freeze-sad-yin
* add hints. specially to the double doors before piano.

hall00
* do something with the cubes of ch3. make them visible but unusable? or lock the door? but how to tell the player that is available now?

ch00
room 3
* mirror, dialogs
* toilet tank, leaks
* shower, leaks?
* inter/story: leaking toilet "i cant invite friends shame"

puzzles

* add other papers like mom letter scolding. or smth. school grades. unpaid receipts. etc.

* add ghosts spawning on item pick up and saying random crap (with dialog system)

* make new puzzles using interact that are not pickup items (buttons, keycode pads, locks, combination, sequence buttons (simon), etc.) (see system/puzzle doc)

* inter : leaking toilet "i cant iite frienhs shame"
* inter: phone starts ringing " you need to pay the X or you cant keep comming, your dad owes 3 months"
* inter/story: phone starts ringing " you need to pay the X or you cant keep comming, your dad owes 3 months"


later
* LInteract: move from the deprecated stock properties to the new autodialogs
* use chap feature to activate interacts that exists across chapters (or step started) (?)
** see PuzzleI06
* refactor: MAYBE: modify inters that show on certain chapter to show on multiple chapters (if needed) but activate only on the correct chapter. to use the room_dl
* * if something can be triggered on any chapter, just use the flags to know if it was activated (see APuzzleI07) UseActiveOnce.
* * think about it.
* * ch0: tv, bed, puzzle

# boss
all
* use the shake more, maybe
* add worldtext

ch04
* the whole chap are boss

ch03
ch02
ch01
ch00

other

# story (and dialogs) (see dialogs file)
## general
* REMEMBER THE THEME FOR THE CHAPTER!
* remember chars mirror adults
* remove bold dialog text style tag (ready: ch00, ch04)
* rename objects (labels)
* add world-texts
* later: replace ones from LInteract::UseItemDlgs and others


## fix

ch00:

## all:


ch04 - Acceptance - Union - All
* find BETTER quote/subtitle for the card acceptance
* add C4_Look (used when pick up, also redirect for looking at it) (Now: Item.Look.C4)
* story: another dialog for pot once i drop something and something else is needed.

chap 03
* PZ06xC00 PZ06xC01 PZ06xC02
* D13_L, D06_L, Inter.Trigger.T03
* maybe one of the puzzles could give a hint on how to open the door, or play the piano, or give an item
* C3S1 improve
* * one of them read a piece of "ridi pagliaccio"
	(CharRow="Main",Text=NSLOCTEXT("[239F452D4861A241E549CD782E7F4EA2]", "452657A0240448B0B8BF0A278CD7C055", "Ridi, Pagliaccio, e ognun applaudirà!"),Condition="!{Inter.Look.TapeI02}",Comment="")

chap 02
* improve hints to use card with door
* remove the call00_T.1 "something fell into the table" from diag datatable
* * also should be onto
* arch: ch 02 or 03. fix the tape usable through the cabinet

chap 01
* story: ch01 poem text. split on multiple pages
* story: ch01 change start cam to be next to door as if sitting
* story: change intro dialog that says stuff in () to be in 1st person from char in present, not future ("()" means a thought). also don't make it super confident, add a ton of doubt to it.
* c1s2 split in two steps
* find quote/subtitle for the card whatworks, also add to the pickup dialog.
* add dialog c0 with puzzle on chap01
pz01xc01
* spot clout, add more dialogs to drop, make it random (?).
* add random dialog on c1spot 00 trigger

ch00
* dialog on the mirror
* story: some diags with the phone wrong numbers

improve

## overview 
ch00 intention - flight - fear - char22
ch01 what works - fight - anger - char18
ch02 'Remove - Yin' - freeze - sad - char07
ch03 Perseverance - fawn - shame - char 11

## puzzles
* picture frames
* puzzle cubes
* phone

## later
* dialogs on other elements. just commenting... some cptsd based remark
** if only as a way to read the book by reading examples of what it looks like. also have inter char comms.
* rename dialog BattsPick to the more generic way, needs to change on BattsI00 and the dialogs.
* add nods to other media in boook titles and tapes
'furidashita ame ga itsuka ymun da ne ....' 'ridi pagliacci'
main books
dd

# sound
remove the attenuation on the sfxs or they won't play with not attached.
* on the sfx files. the ALInteract has an attenuation setting with no occlusion.
* fix music levels (it clips!) (is this a linux thing? (test windows build on windows)

* sound for lswitch04
* sound for heater
* picture?
* sound for ASpot02

add effects
* tv : on, click
* more clout sounds

later
* general: audio volumes
* footsteps ?
* faucet close (sonnis doesn't have a sound)
* no ui. leave for later slices. since i will have to add to all ui.
* maybe: add high/low pass filter to noises when fb > some thresh.

# art
all
* make more rooms
* some accesories, like paper notes, pictures,

c04
* redo rice plate
c03
c02
c01
c00

later
* art fix: boxers have an issue. re-export?
* make a bigger (wider) wardrobe
* not: accessories (not worthy, limit to story relevant for now)

materials
* remove glasssemi_nmi use glasssemi_nDmi (why??)

settings
* base checkbox for settings
* base button, fix on intro
** check common ui

palette: in ue the color should be in the srgb thing. and preview in srgb should be enabled. when using the png palette.

# performance (see performance file)
always as i go
* lower tickinterval for canimator for _specific_ stuff. (leave 60 as default)
* disable ticks, set use attach parent bounds, use auto-manage attachment, add csignificance to animators and ticks
* use clsignificance, add to interacts that can animate far away (not on linteract) (e.g. lights, tv, basin, looping animations)

* shadows https://forums.unrealengine.com/t/fixing-virtual-shadow-map-bugs-in-unreal-engine-5-and-lumen/1339914

* move arch/static assets to room data layers

# build (see build file)
each time:
* make build checklist on pc
** set version
** set packaging: for distribution (saves 30 mb)
** check default flags
** check datalayers
** build hlod and lights (check force no precomputed lights on world settings)
** package for shipping. linux first.
** qa.

* on Steam make sure to use -UserDir=./Data

* package: make a script to exclude content from the demo.

later:
* remove DirectoresToAlwaysCook and test the music again
* fix rebuilding the engine on shipping (on linux on commandline)

* in case of problems remember i've changed "Project settings > Engine > User Interface > Allow high dpi in game"

# publish (see pr file)
* publish devlog
* share to get feedback
** who else?
* screenshots

## demo
* set demo flag on LifeDev.Build.cs
* if i need to redo some stuff: 
* * copy the game level
* * open
* * delete steps above c2 inclusive
* * remove datalayers for above c2 inclusive
* on packaging
* * change Game_L for Demo_L

* add account to sendy or mailchimp
* add link to mail list to 
*** end of game
*** itchio, steam, etc

# design
* go over this file and cleanup
* read the other files (dump, systems, meta)
* redo narrative
** story outline. see video from mata and heroe's journey.
* decide new cycle's direction

design: * define things i want to communicate that will inform story, narrative, items, etc
eg * 'everything is hard' * noone will help * everything is confusing 
* 'whats this about?' 'what do i want to say' 
"* watch vid from patrick bout signs of cptsd and extract bulletpoints, then add situations.

# new features
(but not new mechanics. that's for the next iteration (1.x))

cleanups?
* tobjectptr, likely
* move audio settings to cpp
* dialog ui move stuff to cpp
* * dialog move CurDlgI to cpp
* move outro setflags to cpp

* new: datatable for global dialogs (next sprint)
* * diag subsystem only supports one table. do i extend the diag subsystem? or do i move all my data to one data table?

* fix: disable inventory change while the dialogs are on (this seems be implemented but  not working!)
* improve inventory ui not using regen on mod

settings menu

* add On LFeatsMan:
* * add "fringe" stuff (needs work, also work to save it and apply to the post process, on change and reload)

* improve button styles. use jbutton more
* improve the intro ui
* improve outro ui

# later:
* new card logic (maybe next sprint)

* fix: backlog animation. can i even fix this?

* userdir, find how to hardcode with the define.

* maybe new: inventory add ability to 'see' items. show a mesh in 3D in front of the user. and can be rotated. 
** mesh needs to be an actor
*** to be specified on the data table
** needs to go on the inventory man. or maybe the lchar?

* new: implement G_DEMO (maybe some settings too.)
* new: Implement G_KIOSK (e.g. disable (some) settings, maybe savegame?, reduce playtime?, simplified intro level, shows a video when in the intro for too long (with CTA))
* new: backlog: fix anim not working (using the regular style)
* new: have other human like ghosts walking only visible on fbside
** transparent shader

* debug screen
** freecam: fix going back to menu.
** ( not important) show current step on ui somewhere persistently (maybe on debug window in settings)
** (nopes, console) skip story step
** (nopes console) flashback value

* translate to spanish.
* * enable lang change

* publish plugins to fab
** add filterplugin.ini
** disable modules on uplugin
** disable installed
** update urls

*later: maybe: could rework a bit how the item usable works, with self usable.. like using as enum UseType (usable, use self, none). or maybe not. is ok as is.

# ideas (move to dump/mechanic file):

* make cards unusable when flashback > .7
** needs a way to reduce fb by player?
** careful not to break end of chapter 1
** maybe: fb allow the player to use items but not move ?

* idea: text on the walls that only are visible on the flashback. using the fbside material. or text on the ghosts saying those things.
* idea make the poems usable. reduce fb, with a higher cooldown. it would get boring to re-read the same poem every time? nopes, the dialog does not trigger on trigger, only on Look*. also i can change the dialog per chapter or do something with a custom itemlogic (you're awesome jero!)
** i DO want to give some use to the walkman as item, or the batts, or the poems. (maybe to a way to decrease the fb?)
** i would have to increase the fb of certain objects. and would need to adjust the randomfb.
** i can play by disabling some items at certain moments.
** i've already implemented the ability to change locked!! (thanks jero you're amazing!)
** maybe i can use the card "whatever works" to unlock some poems and stuff.
** if each card is tied to an emotion, maybe by triggering things that increase those emotions, once reached the limit i can trigger that card.
*** i need something on the level that's consumable but infinite that can trigger those things. the player would need a ton of experience to pick up that mechanic. also does the mechanic has some meaning? or is just a mechanic?

* have some flag to track the emotions. do something with this. increase/decrease with some stuff. then do something. trigger an event, activate a card, unlock something. trigger a dialog. die. (maybe on trigger of related card?)

* have some interacts that are only interactable when the fb reveals them
* * given that interacts are based on distance, the fb would need to be pretty high.
* have physics that depends on the fb. like bridges, and door blocks, etc.
* * would be the opposite, would need a lower fb so that they don't disappear or disappear
* make dialog/inventory custom data field? (maybe not)


not:
* idea: not: add non item objects in the drawers (pencils? also create new objects). maybe not. focus on gameplay elements first ?
* leave tapes on the inventory ? 
(maybe not, it will confuse the user, also it will duplicate what the batteries does. i could do something like with the poem. but it could be confusing. it could trigger a dialog, but that dialog can be triggered on pickup or usage).
* not: have multiple datatables for dialogs at the same time.

* no: idea: maybe not. have dialogs that are events. (don't need it. adds clutter to the code).
* no: idea: should i add an autodiag for when a flag is modified?, and/or when an item is modified?
** nopes. because the flags are modified by interacts and items, that already triggers events. and items are rewarded by interacts. 
** also flags and diags are on similar level. at the end of the chain. otherwise it could create a loop. E.g. Diag Shown modifies a flag.

# ////////// D O N E \\\\\\\\\\
* package without pack and see why so fat
* check loadcsvtable json stuff
* Make devlog
* fix lights off count. change to add instead of substract, change the dialog.
  ( should be working but it's not).
* test the build
* update plugins
* fix tape
* fix door hint~~~~
* upload builds
* new: make sure itemsfinish and flagsfinish work together. that it's and and not or
* fix: add feature flag for showing the status bar, on game settings.
* new : add to the overlay ui (or game ui) debug info. mostly timer and stat flags. maybe something else.
* * have a flag to toggle that.
* new: on step, if fading, wait for dl to be loaded before unfade (and calling start)
* * fix, door on room 0 : dont set hint by default. set the hint on c1s0
* deprecate unlockitemreq in favor of the condition
* * fix: the friking black cover overlay. should be set from game mode
* * fix: maybe fix the fade in case it's the first step?
* fix: move the status stuff from the overlay to the game ui
* new: load the base datalayer on the gamemode instead of the steps
* new: add TimeFade to the anim subsystem. replace from SettingsUi.
* fix: move debug feature flags to debug screen. not on game.
  ** add to the settings a field to send feedback.
  ** add a subsystem to funnel all calls through
  *** make instance subsystem
  *** add funtion for breadcrumb
  *** on feat change add/remove tag for feat.
  *** do on the sentry subsystem not on feats man
  *** subsystem gets inited by gameinstance
  *** gets the real sentry subsystem and sets other tags and params
  *** add function for tag
* new : use ddx ddy for outline and watch performance
* make better water parts using refraction
