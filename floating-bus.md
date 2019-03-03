Backup of: http://sky.relative-path.com/zx/floating_bus.html

The Definitive Programmer’s Guide
to Using the Floating Bus Trick
on the ZX Spectrum;
or, How to Keep Your Bus Afloat

Floating bus


This document is a draft. As time passes, and as more questions arise (and as I see fit), I will expand and revise it. So, watch this space.


My other ZX Spectrum–related stuff

1. Introduction
In the con­text of the ZX Spec­trum, the float­ing bus trick refers to ex­ploit­ing a hard­ware quirk of these ma­chines, where a value being read by the ULA from the data bus can also be read by the CPU. (For a more de­tailed de­scrip­tion of the float­ing bus in gen­eral, I en­cour­age you to search the In­ter­net for this term.) As one of ULA’s pri­mary pur­poses is to form the video sig­nal for the TV, the val­ues it reads from the RAM are those that form the bitmap of a byte on the screen and its color over­lay—or at­trib­utes, as they are re­ferred to. Since only one de­vice can ac­cess RAM at a time, and the TV raster beam can­not be sus­pended, the ULA then holds the CPU by sus­pend­ing its clock, reads a value from the RAM, then re­sumes the CPU clock puls­ing. This phe­nom­e­non is com­monly known as mem­mory con­tention. While the dis­play file on the Spec­trum is only ~7KB is size, the con­tention is ap­plied to a full 16K page of RAM (or pages on 128K ma­chines).

The ULA uses a reg­u­lar pat­tern for fetch­ing val­ues from the dis­play file so as not to hold the CPU for the en­tire du­ra­tion of a frame. The pat­tern is as fol­lows: (a) when the ULA is draw­ing the bor­der, no con­tention is ap­plied, and the CPU op­er­ates at full speed; (b) when the ULA is draw­ing the main screen area, it sus­pends the CPU for 4 T states while per­form­ing four reads—bitmap, at­tribute, next bitmap, next at­tribute—then re­leases the CPU again for 4 T states, then con­tin­ues with the next four bytes, and so on and so forth, until it reaches the end of the line and pro­ceeds draw­ing the bor­der again.

The value being read by the ULA from the dis­play file can also be read by the CPU using the IN in­struc­tion and polling any port that is not phys­i­cally at­tached to any pe­riph­eral. Since on the Spec­trum, port ad­dresses are only par­tially de­coded, it is enough to spec­ify the LSB of the port ad­dress. What’s more, dur­ing the idling in­ter­val (i.e. when the ULA is draw­ing the bor­der or dur­ing the 4 T states in be­tween the bitmap/at­tribute byte fetch), all eight lines of the bus are prop­erly held at a log­i­cal “1” thus re­turn­ing the value of 0xFF.

Thus, by read­ing from a port, one can tell whether the ULA is draw­ing the bor­der/idling, or draw­ing the screen.

2. Prac­ti­cal use
While the above might seem overly tech­ni­cal to some of you, the main point to take home is that since we can peek at the value the ULA is cur­rently read­ing (or, more ap­pro­pri­ately, the value cur­rently sit­ting on the data bus), we can also know what area of the screen is being drawn by the TV elec­tron beam. Well, in the­ory.

In prac­tice, we only know if the value on the bus is 0xFF or some­thing else. This might seem like very lit­tle in­for­ma­tion (what if our screen is filled with 0xFF val­ues in both the bitmap and at­tribute areas?), but by care­fully prepar­ing the screen and re­fin­ing the tim­ing of our port read­ing tech­nique, we can get very pre­cise.

3. Going on a tan­gent
Why is this use­ful to begin with? If you want a steady frame rate in your game and no flick­er­ing, you must have some form of syn­chro­niza­tion avail­able to you. Oth­er­wise, the part of your code that is eras­ing an area of the screen be­fore draw­ing new sprites might co­in­cide with in time of the elec­tron beam run­ning across the same area. If you’re a lit­tle un­lucky, that area will flicker oc­ca­sion­ally. If you’re re­ally un­lucky, how­ever, it might re­main blank most or all of the time.

The most straight­for­ward method is to rely on the in­ter­rupt for syn­chro­niza­tion. In­ter­rupts are gen­er­ated by the ULA at the end of each frame. This is use­ful, as we can start ex­e­cut­ing our re­draw­ing rou­tines right after an in­ter­rupt oc­curs. This will give us the time to up­date the screen while the elec­tron beam is draw­ing the top bor­der. If the top of the screen is oc­cu­pied by a more or less sta­tic image (a score board, sta­tus dis­play, fancy or­na­ment, etc.)—even bet­ter, we have some extra time to fin­ish up­dat­ing the screen below it. But what if we want to use the top of the screen and not all of our draw­ing rou­tines fin­ish ex­e­cut­ing by the time the ULA fin­ishes draw­ing the top bor­der?

One method to em­ploy is such case is “draw­ing be­hind the beam.” The con­cept is very sim­ple: you wait for an in­ter­rupt, then wait some more (say, via a wait loop) for the ULA to fin­ish draw­ing the top bor­der, then you start draw­ing or re­draw­ing your screen. Chances are you’ll al­ways stay be­hind the beam, and no flicker will occur.

While vir­tu­ally fool­proof, this method is in­cred­i­bly waste­ful. Here’s where the float­ing bus trick comes into play.

4. There and back again
Imag­ine you could catch the elec­tron beam at any point in time, and sync your draw­ing rou­tines to it. In­stead of wast­ing pre­cious CPU cy­cles be­tween an in­ter­rupt and the be­gin­ning of the dis­play area, you could use it for some other cal­cu­la­tions, then chase the beam as you see fit. Well, you can. If you re­place your idle loop with a loop that reads the data bus and, as the value read changes from 0xFF to some­thing else, you know that it has left the top bor­der and is now in the screen area. That’s very use­ful, but can be taken even fur­ther.

How about sync­ing to an ar­bi­trary po­si­tion on the screen? What if you could start ex­e­cut­ing your screen up­dates even be­fore a frame began, say, at the bot­tom of the screen? That would give you yet more time—the en­tire bot­tom bor­der (at least) plus the top bor­der. By Grabthar’s Ham­mer, what a sav­ings!

Re­mem­ber that read­ing the data bus while the ULA is fetch­ing a bitmap or at­tribute byte will re­turn that very byte. Read­ing the bitmap byte doesn’t seem all that use­ful—we’ll get too many false pos­i­tives, but the at­tribute byte is a whole dif­fer­ent ball­game.

If we fill an area of the screen with a unique at­tribute value—a value that doesn’t ap­pear any­where else on the screen—we can then read the bus in our loop and only exit it when the value read matches the ex­pected one.

5. A work­ing ex­am­ple
Be­fore we begin, I need to warn you about an im­por­tant (al­beit, prob­a­bly, ob­vi­ous at this point) caveat. If we ar­bi­trar­ily read from a port at any time, we’ll end up fetch­ing any of the three val­ues: 0xFF, dur­ing the idling pe­riod (i.e. while the ULA is draw­ing the bor­der or in be­tween the bitmap/at­tribute fetch cycle); the bitmap byte (which can also be 0xFF); or the at­tribute byte (which can also be 0xFF, if we’re crazy enough to set it to white ink on white paper, BRIGHT 1, FLASH 1). We can make sure the at­trib­utes are unique and not 0xFF, but we can’t very well con­trol the bitmap byte. It can be any­thing—that’s the idea of draw­ing after all. For this rea­son, we must en­sure that we only read the at­tribute byte. How, though?

This task is ac­com­plished by care­fully tim­ing our loop. Sev­eral things must be taken into ac­count here, in­clud­ing the fact each in­struc­tion takes a fixed num­ber of T states. More­over, our code must sit in non-con­tended mem­ory, so as not to throw our tim­ing off. There are many ways to skin a cat, and your pre­ferred method of read­ing from a port might be dif­fer­ent from my ex­am­ple below. In my loop, I chose DEC HL as a padding in­struc­tion. It takes 6 T states and doesn’t do any­thing de­struc­tive as far as this par­tic­u­lar piece of code it con­cerned, other than en­sur­ing that we al­ways read the at­tribute byte and not the bitmap byte.

Any port that is not phys­i­cally con­nected to a pe­riph­eral de­vice will do. It’s “tra­di­tional” to use port 0xFF on the Spec­trum. Be­cause of par­tial port ad­dress de­cod­ing, it’s also not par­tic­u­larly im­por­tant what the MSB is (how­ever, con­tention af­fects the tim­ing). I chose port 0x40FF in my ex­am­ple.

As­sem­ble the fol­low­ing code any­where in non-con­tended RAM.


	ld a,9			;INK 1; PAPER 1; BRIGHT 0; FLASH 0
	ld b,32
	ld hl,$5a40		;draw a 32 char-wide strip
1$	ld (hl),a
	inc l
	djnz 1$

fl_bus
	ld de,$940		;attr into D, MSB of port addr into E
2$	dec hl			;[6]padding in­struc­tion
	ld a,e			;[4]MSB of port addr into A
	in a,($ff)		;[11]read port 0x40FF into A
	cp d			;[4]is it D (i.e. INK 1; PAPER 1; BRIGHT 0; FLASH 0)?
	jp nz,2$		;[10]no? keep try­ing
	
	ld a,1			;bor­der blue
	out (254),a
	
	ld b,128		;wait
	djnz $
	
	ld a,7			;bor­der white
	out (254),a
	
	jr fl_bus
When you run it, you should see some­thing like this:


Floating bus trick on 48K/128K/+2


Note the miss­ing line in the left bor­der be­fore the screen area. That’s to be ex­pected—we don’t start chang­ing the bor­der color be­fore we fetch the first at­tribute byte.

If you’re still not con­vinced that we’re only latch­ing onto the at­tribute byte, the ex­am­ple below will de­fin­i­tively demon­strate that it is in fact so. We’ll fill the en­tire bitmap area of the dis­play file with the same value as our at­tribute byte—9.


	ld a,9			;INK 1; PAPER 1; BRIGHT 0; FLASH 0
	ld b,32
	ld hl,$5a40		;draw a 32 char-wide strip
1$	ld (hl),a
	inc l
	djnz 1$

	ld bc,6143	
	ld hl,$4000
	ld de,$4001
	ld a,9			;fill pat­tern
	ld (hl),a
	ldir

fl_bus
	ld de,$940		;attr into D, MSB of port addr into E
2$	dec hl			;[6]padding in­struc­tion
	ld a,e			;[4]MSB of port addr into A
	in a,($ff)		;[11]read port 0x40FF into A
	cp d			;[4]is it D (i.e. INK 1; PAPER 1; BRIGHT 0; FLASH 0)?
	jp nz,2$		;[10]no? keep try­ing
	
	ld a,1			;bor­der blue
	out (254),a
	
	ld b,128		;wait
	djnz $
	
	ld a,7			;bor­der white
	out (254),a
	
	jr fl_bus
As­sem­ble and run it again and you’ll see that the code still only catches the at­tribute byte:


Floating bus trick on 48K/128K/+2


So far, I haven’t told you any­thing you (or Spec­trum de­vel­op­ers at large) didn’t know. Now let’s get into the mys­tery of why the above will work on the 48K/128K/+2 ma­chines, but not on the +2A/+3.

6. It only took us 30 years
The ex­is­tence of the float­ing bus (and the way to ex­ploit it in games) was pre­sum­ably dis­cov­ered by ac­ci­dent by the late, great Joffa Smith in around 1986. The ear­li­est ex­am­ples of games that used this method (al­though in a more prim­i­tive way of wait­ing for the value to change from 0xFF—the bor­der area—to some­thing else) were Cobra and Terra Cresta, both of which came out in late 1986. Very few games took ad­van­tage of this tech­nique since then. My un­der­stand­ing is that it was be­cause it was dis­cov­ered a lit­tle too late. By that time Am­strad had al­ready ac­quired the Spec­trum line of prod­ucts from Sin­clair, and soon af­ter­ward the ZX Spec­trum +2A was re­leased. Its re­designed hard­ware in­tro­duced sev­eral in­com­pat­i­bil­i­ties for the soft­ware that was writ­ten for the orig­i­nal ma­chines (in­clud­ing the +2, which ar­chi­tec­turally was, for all in­tents and pur­poses, a 128K with a slightly dif­fer­ent ROM). The float­ing bus fell vic­tim to one of such re­designs, as it was gone from the +2A and +3.

A cou­ple of ti­tles were rere­leased with the float­ing bus loop re­placed with a sim­ple idle wait loop, some­times with not ill ef­fect. Cobra, how­ever, suf­fered from a great deal of flick­er­ing, much to Joffa’s cha­grin. All in all, the tech­nique was aban­doned, since it could not be used on all Spec­trum. Or could it?

That was the con­sen­sus for al­most thirty years. In early 2016, Cesar Her­nan­dez, the au­thor of the ZE­sarUX Spec­trum em­u­la­tor, made an in­ter­est­ing dis­cov­ery. Ac­cord­ing to his sim­ple test (writ­ten in BASIC), some­thing of a float­ing bus was in­deed pre­sent on the +2A/+3, al­beit with a few dif­fer­ences from the pre­vi­ous mod­els. Namely, it oc­curred on dif­fer­ent ports, when pag­ing was en­abled, and the re­turned value al­ways had Bit 0 set. Some spec­u­la­tion fol­lowed, but no­body felt com­pelled enough to take it any fur­ther.

Fast for­ward to the fall of 2017, when yours truly stum­bled upon Cesar’s post. Co­in­ci­den­tally, at that time I was strug­gling with shoe­horn­ing my draw­ing rou­tines for A Yan­kee in Iraq into the top bor­der. No mat­ter how hard I tried, there still was a nar­row strip at the top of the screen where flicker could occur some­times. Much like the rest of the com­mu­nity, I was under the im­pres­sion that the float­ing bus trick was a pe­cu­liar quirk that only ex­isted on ear­lier Sin­clair ma­chines and as such was of no use if you wanted your game to run on all Spec­trums.

Cesar’s post made me very cu­ri­ous. What if . . . ? That’d be great news, I thought. But per­haps it’s wish­ful think­ing, since no one has used it since then. Cu­rios­ity and stub­born­ness took the bet­ter of me, and I began my own in­ves­ti­ga­tion. The rest is his­tory. Suf­fice it to say that with the help of a few vol­un­teers and some ad­di­tional tests by Hikaru (his were much more sci­en­tific than mine), we man­aged to solve the Great Float­ing Bus Mys­tery on the +2A/+3. It only took us thirty years. In the process, we also helped Mark Wood­mass make the nec­es­sary changes to his SpecEmu, which be­came the first Spec­trum em­u­la­tor to fully sup­port the float­ing bus on all Spec­trums. A Yan­kee in Iraq be­came the first Spec­trum game to take ad­van­tage of the float­ing bus trick across the en­tire Spec­trum line of com­put­ers.

Why the his­tor­i­cal tan­gent, I hear you ask? Be­cause as of this writ­ing, SpecEmu by Mark Wood­mass and Spec­tramine by weiv, are the only two em­u­la­tors that I’m aware of that you can use to test your games. Both are Win­dows-only. I was only able to per­fect the tech­nique by pur­chas­ing an ac­tual Spec­trum +2A, so un­less you own one or trust the afore­men­tioned em­u­la­tors com­pletely, I strongly sug­gest you fol­low my in­struc­tions to the let­ter.

7. Enough chit-chat. Let’s dig in
The float­ing bus on a +2A/+3 dif­fers from pre­vi­ous Spec­trums in a few key as­pects out­lined below:


1. It is only found on ports that fol­low the pat­tern (1 + (4 * n) && n < 0x1000) (that is, ports 1, 5, 9, 13 . . . 4093).

2. The bus al­ways re­turns 0xFF if bit 5 of port 32765 is set (i.e. pag­ing is dis­abled), so it won’t work in 48K mode.

3. Oth­er­wise, the value re­turned is the value cur­rently read by the ULA ORed with 1 (i.e. Bit 0 is al­ways set). In prac­ti­cal terms, this means that we can’t use even val­ues for the INK at­tribute; so no black, red, green, or yel­low.

4. Dur­ing idling in­ter­vals (i.e. when the ULA is draw­ing the bor­der or in be­tween fetch­ing the bitmap/at­tribute byte), the bus latches onto the last value that was writ­ten to, or read from, con­tended mem­ory, and not strictly 0xFF. This is cru­cial to keep in mind, but I’ll ex­plain it in de­tail below.


Quite a laun­dry list, if you ask me. That’s prob­a­bly an­other rea­son why it was not dis­cov­ered until later. Com­mer­cial soft­ware au­thors were not in­sane enough to spend extra time and ef­fort going on a wild goose chase in­ves­ti­gat­ing it to such an ex­tent. Leave it to a cou­ple of in­ces­sant Spec­trum en­thu­si­asts in their for­ties to tackle the de­tails.

So, how do we mod­ify our code above to work on a +2A/+3, then? Let’s go through the check­list above, one item at a time. First, the port ad­dress. That’s easy enough. For no par­tic­u­lar rea­son other than this au­thor’s whim, we’ll chose port 4093 (0xFFD). Check. Make sure pag­ing is en­abled. Well, as long as we don’t dis­able it ex­plic­itly (and the user doesn’t de­cide to switch to 48K BASIC mode), we’re good. Upon power-up, the Spec­trum will have pag­ing en­abled. Check. We al­ready made sure that Bit 0 is set in the value that we’re ex­pect­ing (INK 1). Check. Now comes the tricky bit.

Two con­di­tions must be met si­mul­ta­ne­ously. One is that, as in the pre­vi­ous ex­am­ple, the tim­ing of our loop has to be just right so we don’t ac­ci­den­tally fetch the bitmap or idling bus value. The other is that we can no longer rely on the fact that the idling value will al­ways be 0xFF. It can be any byte (ORed with 1, re­mem­ber) that was placed on the data bus if it was read from, or writ­ten to, con­tended mem­ory. So as not to leave it to chance, we must “pre­load” it with a value that is dif­fer­ent from our sync value our­selves. After many days of ex­per­i­men­ta­tion in Sep­tem­ber of 2017 (and only after I had di­rect ac­cess to an ac­tual +2A), I ar­rived at the sim­plest padding in­struc­tion: LD A,(NNNN), where NNNN is any ad­dress in con­tended mem­ory. Again, on a whim, I chose the first at­tribute byte of the dis­play file 0x5800.

Let’s rewire our ex­am­ple.

```
	ld a,9			;INK 1; PAPER 1; BRIGHT 0; FLASH 0
	ld b,32
	ld hl,$5a40		;draw a 32 char-wide strip
1$	ld (hl),a
	inc l
	djnz 1$

fl_bus
	ld de,$90f		;attr into D, MSB of port addr into E
2$	ld a,($5800)		;[13]point to con­tended mem­ory and fetch a “blank­ing” attr
	ld a,e			;[4]MSB of port addr into A
	in a,($fd)		;[11]read port 0xFFD into A
	cp d			;[4]is it D (i.e. INK 1; PAPER 1; BRIGHT 0; FLASH 0)?
	jp nz,2$		;[10]no? keep try­ing
	
	ld a,1			;bor­der blue
	out (254),a
	
	ld b,128		;wait
	djnz $
	
	ld a,7			;bor­der white
	out (254),a
	
	jr fl_bus
```
If all goes well, after as­sem­bling and run­ning this list­ing, you should see a pic­ture sim­i­lar to the one in our first ex­am­ple. There are some tim­ing dif­fer­ences be­tween +2A/+3 ma­chines and the pre­vi­ous mod­els. The for­mer apply con­tention only if the MREQ sig­nal is low (there­fore mak­ing I/O ac­cess non-con­tended), whereas the lat­ter apply it every time the ULA needs to ac­cess any area of con­tended RAM. This ex­plains why, say, games that use the mul­ti­color trick must use dif­fer­ent pat­terns for each ma­chine: one for the 48K, one for the 128K/+2 (be­cause of a dif­fer­ent clock speed), and one for the +2A/+3 (be­cause con­tention pat­terns are dif­fer­ent). But that’s an aside that is not re­lated to this ar­ti­cle.




Second draft: April 18, 2018 by Ast A. Moore (added a bitmap fill pattern)

First draft: April 17, 2018 by Ast A. Moore



