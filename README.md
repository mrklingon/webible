# webible

World English Bible Assistant + KJV, KLV, Douay, Vulgate and Mandoa
-----------------------------------

Bibles Archive 

 * bibles/drv.zip - Douay 
 * bibles/kjvgbf.zip - King James
 * bibles/klvgbf.zip - Klingon Language Version
 * bibles/mndgbf.zip - Mandoa Language
 * bibles/vulgate.zip - Latin Vulgate
 * bibles/webgbf.zip - World English Bible

    Set variable FHOME=./ to the location to find the Bible directories above.
    
    Simple tool to read and use the World English Bible, and more.
    
    Command Line mode:
    
    READ BOOK:
    
    webible  book|book-number [chapter [verse  [end-verse] ] ]
    
    kjbible  book|book-number [chapter [verse  [end-verse] ] ]
    
    drbible  book|book-number [chapter [verse  [end-verse] ] ]
    
    For example:
    
    	webible Isaiah 7 14 15
    
    Looking for Isaiah 
    7:14    weary my God also? | Therefore the Lord himself will give you a sign:
    7:14    behold, a virgin shall conceive, and bear a son, and shall call his name
    7:15    Immanuel. | Butter and honey shall he eat, when he knows to refuse the


INTERACTIVE mode:
============
    $ webible
    
    WEBible : World English Bible Assistant
    
    webible - print portions of the WEB Bible
    
    Usage:
    	book [chapter [verse  [end-verse] ] ]
    	>filename - write output to filename
    	!command  - execute command
    	web       - Switch to WEB translation
    	kjv       - Switch to KJV translation
    	drv       - Switch to DRV translation
    	lat       - Switch to DRV translation
    	*quit     - end program
    	*wrap 99  - set wrap at column 99 (set to large number to turn off
    	?         - this summary
    	*books    - list books
    
    
    WEBible Assistant for the World English Bible - 4.0
    Joel Peter Anderson, January 1st, 2002
    WEBible> 

=====================

    You can use this to display any passage.  If you direct output to a file
    then you can save portions to edit, review or print the selections.  One
    handy feature is to write some pieces to a file, then call them up in an 
    editor:
    
    $ ./webible
    WEBible : World English Bible Assistant
    
    ./webible - print portions of the WEB Bible
    
    Usage:
    	book [chapter [verse  [end-verse] ] ]
    	>filename - write output to filename
    	!command  - execute command
    	*quit     - end program
    	?         - this summary
    
    WEBible Assistant for the World English Bible - 1.0
    Joel Peter Anderson, February 19, 1999
    
    WEBible> >messiah.txt          [Start writing file "messiah.txt"]
    
    WEBible> Isaiah 7 13 15        [view Isaiah 7:13-15]
    
    Looking for Isaiah 
    7:13    I will not ask, neither will I tempt Yahweh. | He said, “Listen now,
    7:13    house of David: Is it a small thing for you to weary men, that you will
    7:14    weary my God also? | Therefore the Lord himself will give you a sign:
    7:14    behold, a virgin shall conceive, and bear a son, and shall call his name
    7:15    Immanuel. | Butter and honey shall he eat, when he knows to refuse the
    
    WEBible> Genesis 3 14 16       [view Genesis 3:14-16]
    Looking for Genesis 
    3:14    | Yahweh God said to the serpent, “Because you have done this, cursed are
    3:14    you above all cattle, and above every animal of the field. On your belly
    3:15    shall you go, and you shall eat dust all the days of your life. | I will
    3:15    put enmity between you and the woman, and between your offspring and her
    3:15    offspring. He will bruise your head, and you will bruise his heel.”
    3:16    | To the woman he said, “I will greatly multiply your pain in childbirth.
    3:16    In pain you will bring forth children. Your desire will be for your husband,
    3:16    and he will rule over you.”
    
    WEBible> !write messiah.txt    [view file messiah.txt using "write"]
    WEBible> *quit                 [end program]


BBuild.pl
---------------------------
BBuild - build Bible podcasts.

Usage: BBuild subject verse-list-file

Script generates two files:

   subject-klingon-out.html   coded html for A Klingon Word from the Word
   subject-mandoa-out.html coded html for A Mando'a Word for a Warrior.
   
   html files can be edited for lead-in sentance, prior to recording and then pasted into Blogger entry for posting.
   
bfilt
----------
simple sed script used by BBuild.pl to strip extraneous output from webible/klbible/mndbible output.

