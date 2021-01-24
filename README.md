# Timelapse
A MapleStory v83 Trainer

![alt text](https://i.imgur.com/tLDMi6s.gif)

# TODO List

## ToolBar
- Better save/load mechanism, add saving of itemFilter and mapRusher predefined routes
- Code Enable/Disable All
- Code Launch MS client.
- Code Injection routines to deploy the trainer
- Code Rezise and Embed MS client into form of the trainer
- Code Hide/Show MS Window (already coded in old trainer)

## Main Tab
##### AutoLogin
- Code AutoLogin routines either thru packets or by hooking client functions
- Code Pin/Pic bypass
- Code Skip Logo (already found in .CT)
##### Options
- Code disable pointers

## Bots Tab
##### Main
- (ADD) Auto Turn? 
- Code console for output of various logging streams
##### AutoCC/CS
- (ADD) Add option for auto face left/right after cc/cs
- Fix AutoCC tbCCCSTime, tbCCCSPeople, tbCCCSAttack, tbCCCSMob being possibly empty/null (add error message) :: if time/people == 0, change to 1
- Fix Auto CS function call (doesn't work as intended, get rid of blue box popup on MigrateToCashShop Call)
- Make Auto CC/CS run on threads instead of timers. Timers cause the trainer to hang up 
- Add option to select witch channels should be used as random choice of AutoCC and which should be prohibited from entering
- How does actually autoCC and NoBreath relate? Is it valid to send CC packet when having no breath?
##### Advanced Bot
- (ADD) Flat bot
- (ADD) climbing ladders/rope (CVecCtrl::GetLadderOrRope/CVecCtrl::IsOnLadder/CVecCtrl::IsOnRope 
- (ADD) Find nearest mob/most grouped mobs in a platform (spiral algo with rings started from inside (for loop with 2 for loops inside)
- (ADD) level based map rush
- (ADD) walk left/right
- (ADD) mouse emulation autoclicking etc...

## Hacks Tab
- (ADD) Alot of missing hacks BYOR(Bring Your Own Rope!), ItsRainingMobs, MSCRC Bypass, MP/HP regen hack, AttackUnrandomizer etc...
- (ADD) Char Walk & Jump Speed editing(double), damage, and other physics, 
- Auto Assign AP (make sure packet auto ap not enabled) 

## Vacs Tab
- Code DupeX/MMC/Wall Vac (already coded in old trainer)
- (ADD) Vami, Portal Kami, dEMi, CSEAX vac, Mouse CSEAX, Force Vac, Pvac, Vac right, Vac left if possible
- (ADD) Pet Item Vac

## Filters Tab
- (Bug) Not all items are ingame items can be found via string search.
- Modifications?

## Packets Tab
- (ADD) code all block packets
- (ADD) code logging send/recv packets
- (ADD) code multi packet for send & recv
- (ADD) code/find defined packets including Auto AP, Auto-Sell, intercontinental packets if possible 

## Map Rusher Tab
- Code the actual Map Rush part
- (ADD) Make Map Rush intercontinental
- (ADD) Code Auto-Rushback
- (ADD) Code allowing for both packet based and teleport based map rush
- Remake wz xml parser in C++ since there are a lot of portals missing from maps (ie kerning square from subway lobby in kerning)

## Overall
- The GUI needs overall change
- Look into making sure trainer can be injected into multiple instances with no interference between
- Rewrite memory management into proper mememory manager which is type generic and more operation safe.
- Change c style casts into c++ style casts?
- (ADD) check for auto-loot to see if inventory is full, add option to sell useless shit and buy pots 
- (ADD) Make a label visible for a bit (or log) all actions in trainer
- (ADD) Make Tooltips for everything
- (ADD) Logger for mesos picked up/mobs killed/# of levels leveled up during hacking session
- (ADD) countdown for Auto CC/CS on bottom of trainer if active (look into replacing useless time label on bottom right)

## Bugs  
- Fix ClickTeleport() tbClickTeleport, MouseTeleport() tbMouseTeleport, TeleportLoop() tbTeleportLoopDelay being empty
- Check and fix all textbox error messages
- Add spawn point dynamic stuff to Codecave and see if Strucs.h/SpawnControlStruct is necessary ??? maybe in helper function in codecave namespace, loop through listviews and return a dynamically constructed struct/array
- Look into seeing if there is a pointer to see if map has finshed loading (uses: map rush & auto cc/cs), otherwise users can click multiple times, causing the function to complete multiple times before cc'ing is actually finished
- Find better functions to code cave in for map name, hp, mp and exp pointers
- All readpointers/readmultipointers pointer functions should return integer values, caller function should convert to string. Only applicable to number values, not text values
- Look into disabling textbox values if something is checked, also remove all textChanged events
- Fix aesthetics of listviews (accommodating for vertical scrollbars), and make sure all listview code is identical in terms of adding/deleting/searching
- Look into changing names of certain textboxes to add Count (ie tbHPMob should be tbHPMobCount)
- Look into changing names of all textboxes with name "interval" to "delay", makes more sense
- For all new number only textboxes, double check to see if the keypress event (in MainForm.h) is defined
- Check to make sure all char arrays are textual and unsigned char are for memory usage
- Hack Fix: Turned all booleans to false on form closing to prevent maplestory from closing as well, this might not work in the case that the delay for the loop is high, consider keeping all seperate threads in a list and exiting each thread in a better more elegant way
- Account for there being no channel 0 (channel starts at 1 ingame, label already changed, but make sure funcs are change to reflect that)
- Change SendPacket's passed in packet str, in AutoCC/CS, to the packet write methods 

##### HotkeyQue
- The current hotkey handling system is very unresponsive and laggs the entire input system to maplestory.
- Just having auto attack on 200ms and loot on 200ms is sufficient to overspam maple input so much that no else key command will be executed unless agresivelly repeatedly pressed or held down for an ammount of time

##### Stability issues
- FMA
- unstable on large maps in both directions as long or tall
- unstable on maps with flying/jumping monsters
- unstable when u miss or hit too many times till mob dies 

- relatively stable on small maps where mobs die qucikly

- Kami
- the same as above the stability of both seems to increase with timed autoCC

## Tips
- For preventing new line before opening brace:
	Vanilla Visual Studio: [Tools -> Options > Text Editor > C# > Code Style > Formatting > New Lines] Remove the check on all options here to never put the open bracket on a new line.
	Resharper: [Resharper > Options > Code Editing > C# > Formatting Style > Braces Layout] Set all top "Braces Layout" options to "At end of line (K&R style)
- For issues with Resharper marking everything with red:
	[Visual Studio > Tools > Options > Re-sharper > General > Options > General > Clear Caches] [Visual Studio > Tools > Options > Re-sharper > General > Suspend Now > Resume Now]
	If above doesn't work, disable code analysis in Options
- C++/CLI Pointers:
	[Native: P *ptr = new P; // usual pointer allocated on heap | P &nat = *ptr; //Reference to object]
	[CLI: MngdObj ^mngd = gcnew MngdObj; // allocated on CLI heap MngdObj %obj = *mngd; //Reference to CLI Object]
- Use Stealth Inject option in Settings for Extreme Injector
