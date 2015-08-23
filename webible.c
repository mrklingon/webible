 #include <stdio.h>
 #include <string.h>
 #include <libgen.h>

 char *PNAME, *P2NAME;
 char *OPROG;
 char tline[2000];
 char input[2000];
 char cmd[2000];

 #define PROG "WEBible : World English Bible Assistant"
 #define KPROG "KJBible : King James Bible Assistant"
 #define DPROG "DRBible : Douay Rheims Bible Assistant"
 #define LPROG "LatBible : Latin Vulgate Bible Assistant"
 #define TPROG "KLBible : Klingon Language Bible Assistant"
 #define MPROG "MNDBible : Mando'a Language Bible Assistant"
 #define VERS "5"
 #define LVL "1"
 #define DATE "July 26th, 2006"

 int tcnt=5;

 int maxbooks;
 int kjv = 0; /*flag version*/
 int lat = 0; /*flag version*/
 int klv = 0; /*flag version*/
 int mnd = 0; /*flag version*/
 int drv = 0;
 int curbook = 0;
 int curchap = 0;
 int curverse = 0;
 int WRAPCOL = 9999;

 
 /*
  *       _        _    
  *  __ _| |_ ___ | | __
  * / _` | __/ _ \| |/ /
  *| (_| | || (_) |   < 
  * \__,_|\__\___/|_|\_\
  *atok - which crucial TOKE is here?
  */

 char *tokes[] = {"<SB","<SC","<SV","<RF","<Rf"};

 #define BSTART 0
 #define CSTART 1
 #define VSTART 2
 #define NSTART 3
 #define NEND 4


 atok(char *txt){ /*which crucial toke is here?*/
   int i;
   for (i=0;i<tcnt;i++){
     if (!strncmp(txt,tokes[i],strlen(tokes[i]))){
       return i;
     }
   }
   return -1;
 }

 
 /*
  *
  *                     
  *            _ _ _                _     
  *  ___ _ __ | (_) |___      _____| |__  
  * / __| '_ \| | | __\ \ /\ / / _ \ '_ \ 
  * \__ \ |_) | | | |_ \ V  V /  __/ |_) |
  * |___/ .__/|_|_|\__| \_/\_/ \___|_.__/ 
  *     |_|                               
  *
  *  split web - split the web.gbf, kjv.gbf into books
  */
 splitweb (){
   char input[2000];
   char btitle[2000];

   FILE *BIBLE;
   FILE *BOOK=(FILE *)0; /*initially not open*/
   int BKN=0;


   if (kjv){
     printf ("breaking up King James Bible into Directory\n");
     if (BIBLE = fopen ("KJV.GBF","r")){
       system ("mkdir kjv.dir");/*make subdirectory for books*/
       BKN=0;

       while (fgets (input,2000,BIBLE) ){
	 if ( ftok(input,"<SB") > -1){
	   BKN++;

	   if (BOOK){
	     fclose(BOOK);
	   }

	   sprintf(btitle,"kjv.dir/%d.kjv",BKN);
	   printf("making >%s\n",btitle);
	   BOOK=fopen (btitle,"w");
	 }
	 if (BOOK) { fprintf(BOOK,"%s",input); }
       }
       fclose(BIBLE);
     }
   }else{
     printf ("breaking up World English Bible into Directory\n");
     BIBLE = fopen ("WEB.GBF","r");
     if (BIBLE){
       system ("mkdir web.dir");/*make subdirectory for books*/
       BKN=0;

       while (fgets (input,2000,BIBLE) ){
	 if ( ftok(input,"<SB") > -1){
	   BKN++;

	   if (BOOK){
	     fclose(BOOK);
	   }

	   sprintf(btitle,"web.dir/%d.web",BKN);
	   printf("making >%s\n",btitle);
	   BOOK=fopen (btitle,"w");
	 }
	 if (BOOK) { fprintf(BOOK,"%s",input); }
       }
       fclose(BIBLE);
     }
   }
 }

 
 /*
  *         _ _            
  *     ___| (_)_ __   ___ 
  *    / __| | | '_ \ / _ \
  *    \__ \ | | | | |  __/
  *    |___/_|_|_| |_|\___|
  *                    
  *  strip line down to printable version
  */

 sline (char *txt){
   int where = 0;
   char *inp = txt;
   char *out = &tline[0];
 
   tline[0]='\0';

   if (WRAPCOL < 1){
     WRAPCOL = 999999;
   }

   for (;*inp;){
     where++;
     if ( (where >=WRAPCOL) && (*inp == ' ')){
       *inp = '\t'; /* provide line wrap. */
       *out++ = '\n';
       *out++ = *inp++;
       where = 0;
     }

     switch (atok(inp)){
     case BSTART:
       curchap=0;
       curverse=0;
       while (*inp != '>'){
	 inp++;
       }
       inp++;
       curbook++;
       break;

     case CSTART:
       while (*inp != '>'){
	 inp++;
       }
       curverse=0;
       inp++;
       curchap++;
       break;

     case VSTART:
       while (*inp != '>'){
	 inp++;
       }
       inp++;
       *out++ = '|';
       *out++ = ' ';
       curverse++;
       break;

     case NSTART:
       while (*inp != '>'){
	 inp++;
       }
       inp++;

       *out++ = ' ';
       *out++ = '{';
       *out++ = ' ';
       *out++ = 'N';
       *out++ = 'o';
       *out++ = 't';
       *out++ = 'e';
       *out++ = ':';
       *out++ = ' ';
       break;

     case NEND:
       while (*inp != '>'){
	 inp++;
       }
       inp++;

       *out++ = ' ';
       *out++ = '}';
       *out++ = ' ';

       break;

     default:

       if (*inp == 13){
	 inp++;}
       if (*inp == '<'){
	 while (*inp != '>'){
	   inp++;
	 }
	 inp++;
       }else{
	 *out = *inp;
	 out++;
	 inp++;
       }
     }
   }
   *out++ = *inp++;

 }
 
 /*
  *  __ _        _    
  * / _| |_ ___ | | __
  *| |_| __/ _ \| |/ /
  *|  _| || (_) |   < 
  *|_|  \__\___/|_|\_\
  *                   

   return offset for token in line
 */



 ftok (char *txt,char *tok) { /*return offset for token in line*/
   int i;
   char *inp = txt;

   for (i=0;i<(strlen(txt)-strlen(tok));i++){
     if (!strncasecmp(inp,tok,strlen(tok))){
       return i;
     }
     inp++;
   }
   return -1;
 }
 
 /*

  *   _     _                 _    
  *  (_)___| |__   ___   ___ | | __
  *  | / __| '_ \ / _ \ / _ \| |/ /
  *  | \__ \ |_) | (_) | (_) |   < 
  *  |_|___/_.__/ \___/ \___/|_|\_\
  *  isbook - convert book text name to number
 */
 char *DBOOKS[] = { "", "1esdras", "1kings", "1machabe", "1para",
		    "2esdras", "2kings", "2machabe", "2para", "3kings", "4kings",
		    "abdias", "agg", "amos", "baruch", "canticle", "daniel", "deut",
		    "ecclcus", "ecclstes", "esther", "exodus", "ezech", "genesis",
		    "habacuc", "isaias", "jeremias", "job", "joel", "jonas", "josue",
		    "judges", "judith", "lament", "levit", "malachias", "micheas", "nah",
		    "numbers", "osee", "proverbs", "psalms", "ruth", "soph", "tobias",
		    "wisdom", "zacharias", "1corin", "1john", "1peter", "1thes",
		    "1timothy", "2corin", "2john", "2peter", "2thes", "2timothy", "3john",
		    "acts", "apoca", "coloss", "ephesian", "galat", "hebrews", "james",
		    "john", "jude", "luke", "mark", "mat", "philemon", "philip", "romans",
		    "titus","","","",""};

 char *KBOOKS[] = {  "", "Genesis",  "Exodus",  "Leviticus",  "Numbers", 
		     "Deuteronomy",  "Joshua",  "Judges",  "Ruth",  "1Samuel", 
		     "2Samuel",  "1Kings",  "2Kings",  "1Chronicles",  "2Chronicles",  "Ezra",  "Nehemiah", 
		     "Esther",  "Job", "Psalm",  "Proverbs",  "Ecclesiastes",  "Song of Songs", 
		     "Isaiah",  "Jeremiah",  "Lamentations",  "Ezekiel", 
		     "Daniel",  "Hosea",  "Joel",  "Amos",  "Obadiah", 
		     "Jonah",  "Micah",  "Nahum",  "Habakkuk",  "Zephaniah",
		     "Haggai",  "Zechariah",  "Malachi",         "Tobit", "Judith", "GrkEs", "Wisdom", "Sirach",
		     "Baruch", "EpiJeremiah", "Azariah", "Susanna", "Bel", "1Maccabees", "2Maccabees", "1Esdras",
		     "Manasseh", "2Esdras", "Matthew", "Mark", "Luke", "John", "Acts", "Romans", "1Corinthians", 
		     "2Corinthians", "Galatians", "Ephesians", "Philippians", "Colossians", "1Thessalonians", 
		     "2Thessalonians", "1Timothy", "2Timothy", "Titus", "Philemon", "Hebrews", "James", 
		     "1Peter", "2Peter", "1John", "2John", "3John", "Jude", "Revelation"};

  char *BOOKS[]={  "", "Genesis",  "Exodus",  "Leviticus",  "Numbers", 
		 "Deuteronomy",  "Joshua",  "Judges",  "Ruth",  "1Samuel", 
		 "2Samuel",  "1Kings",  "2Kings",  "1Chronicles",  "2Chronicles",  "Ezra",  "Nehemiah", 
		 "Esther",  "Job", "Psalm",  "Proverbs",  "Ecclesiastes",  "Song of Songs", 
		 "Isaiah",  "Jeremiah",  "Lamentations",  "Ezekiel", 
		 "Daniel",  "Hosea",  "Joel",  "Amos",  "Obadiah", 
		 "Jonah",  "Micah",  "Nahum",  "Habakkuk",  "Zephaniah",
		 "Haggai",  "Zechariah",  "Malachi",  "Matthew", 
		 "Mark",  "Luke",  "John",  "Acts",  "Romans",  
		 "1Corinthians",  "2Corinthians",  "Galatians",  "Ephesians",
		 "Philippians",  "Colossians",  "1Thessalonians",  "2Thessalonians", 
		 "1Timothy",  "2Timothy",  "Titus", 
		 "Philemon",  "Hebrews",  "James",  "1Peter",  "2Peter",
		  "1John",  "2John",  "3John",  "Jude", 
		  "Revelation"};
  char *TBOOKS[]={  "", "Genesis",  "Exodus",  "Leviticus",  "Numbers", 
		 "Deuteronomy",  "Joshua",  "Judges",  "Ruth",  "1Samuel", 
		 "2Samuel",  "1Kings",  "2Kings",  "1Chronicles",  "2Chronicles",  "Ezra",  "Nehemiah", 
		 "Esther",  "Job", "Psalm",  "Proverbs",  "Ecclesiastes",  "Song of Songs", 
		 "Isaiah",  "Jeremiah",  "Lamentations",  "Ezekiel", 
		 "Daniel",  "Hosea",  "Joel",  "Amos",  "Obadiah", 
		 "Jonah",  "Micah",  "Nahum",  "Habakkuk",  "Zephaniah",
		 "Haggai",  "Zechariah",  "Malachi",  "Matthew", 
		 "Mark",  "Luke",  "John",  "Acts",  "Romans",  
		 "1Corinthians",  "2Corinthians",  "Galatians",  "Ephesians",
		 "Philippians",  "Colossians",  "1Thessalonians",  "2Thessalonians", 
		 "1Timothy",  "2Timothy",  "Titus", 
		 "Philemon",  "Hebrews",  "James",  "1Peter",  "2Peter",
		  "1John",  "2John",  "3John",  "Jude", 
		  "Revelation"};


char *LBOOKS[] = {"", "1Corinthians", "Ezra", "1John", "1Samuel", "1Macabees",
		"1Chronicles", "1Peter", "1Thessalonians", "1Timothy", "2Corinthians",
		"Nehemiah", "2John", "2Samuel", "2Macabees", "2Chronicles", "2Peter",
		"2Thessalonians", "2Timothy", "3John", "1Kings", "2Kings", "Obadiah",
		"Acts", "Haggai", "Amos", "Revelation", "Baruch", "SongofSongs",
		"Colossians", "Daniel", "Deuteronomy", "Sirach",
		"Ecclesiastes", "Ephesians", "Esther", "Exodus", "Ezekiel",
		"Galatians", "Genesis", "Habakkuk", "Hebrews", "Isaiah", "James",
		"Jeremiah", "Job", "Joel", "John", "Jonah", "Joshua", "Jude",
		"Judges", "Judith", "Lamentations", "Leviticus", "Luke", "Malachi",
		"Mark", "Matthew", "Micah", "Nahum", "Numbers", "Hosea", "Philemon",
		"Philippians", "Proverbs", "PsalmsH", "PsalmsG", "Psalm151", "Romans",
		"Ruth", "Zephaniah", "Titus", "Tobit", "Wisdom", "Zechariah",
		"3Esdras", "4Esdras", "Laodicians", "XPM" };


 isbook(char *book){
   int i;

   if (strlen(book)){

     for(i=1;i<=maxbooks;i++){
       if (!strncasecmp(BOOKS[i],book,strlen(book))){
	 return (i);
       }
     }
   }
   return 0;
 }
 iskbook(char *book){
   int i;

   if (strlen(book)){

     for(i=1;i<=81;i++){
       if (!strncasecmp(KBOOKS[i],book,strlen(book))){
	 return (i);
       }
     }
   }
   return 0;
 }
 isdbook(char *book){
   int i;

   if (strlen(book)){

     for(i=1;i<=73;i++){
       if (!strncasecmp(DBOOKS[i],book,strlen(book))){
	 return (i);
       }
     }
   }
   return 0;
 }
 islbook(char *book){
   int i;

   if (strlen(book)){

     for(i=1;i<=73;i++){
       if (!strncasecmp(LBOOKS[i],book,strlen(book))){
	 return (i);
       }
     }
   }
   return 0;
 }


 
 /*

  *   _   _ ___  __ _  __ _  ___                               
  *  | | | / __|/ _` |/ _` |/ _ \                              
  *  | |_| \__ \ (_| | (_| |  __/                              
  *   \__,_|___/\__,_|\__, |\___|                              
  *                   |___/                                    


 */
 void usage(){

   int i;

   if (kjv){
     printf ("%s\n\n",KPROG);
   }else{
     if (drv) {
	   printf ("%s\n\n",DPROG);
     }else {
       printf ("%s\n\n",PROG);
     }
   }
   for (i=1;i<=maxbooks;i++){
     if (lat){
       printf ("%3d : %15s\t",i,LBOOKS [i]);
       i++;
       printf ("%3d : %15s\t",i,LBOOKS [i]);
       i++;      
       printf ("%3d : %15s\n",i,LBOOKS [i]);
       i;
     } else     if (kjv){
       printf ("%3d : %15s\t",i,KBOOKS [i]);
       i++;
       printf ("%3d : %15s\t",i,KBOOKS [i]);
       i++;      
       printf ("%3d : %15s\n",i,KBOOKS [i]);
       i;
     } else{   
       if (drv){
	 printf ("%3d : %15s\t",i,DBOOKS [i]);
	 i++;
	 printf ("%3d : %15s\t",i,DBOOKS [i]);
	 i++;      
	 printf ("%3d : %15s\n",i,DBOOKS [i]);
	 i;
       }else{
	 printf ("%3d : %15s\t",i,BOOKS [i]);
	 i++;
	 printf ("%3d : %15s\t",i,BOOKS [i]);
	 i++;      
	 printf ("%3d : %15s\n",i,BOOKS [i]);
	 i;
       }
     }
   }


   printf ("\n\n%s - print portions of the Bible\n",PNAME);
   printf ("\n");
   printf ("Usage:\n");
   printf ("\n");
   printf ("%s book|book-number [chapter [verse  [end-verse] ] ]\n",PNAME);
   printf ("\n");

   if (kjv){
     printf ("\n\nKJBible Assistant for the King James Bible - %s.%s\nJoel Peter Anderson, %s\n",VERS,LVL,DATE);
   }else{
     printf ("\n\nWEBible Assistant for the World English Bible - %s.%s\nJoel Peter Anderson, %s\n",VERS,LVL,DATE);
   }

 }
 void usage2(){

   int i;
   if (kjv){
     printf ("%s\n\n",KPROG);
     printf ("\n\n%s - print portions of the KJV Bible\n",PNAME);
   }else{
     if (drv){
     printf ("%s\n\n",DPROG);
     printf ("\n\n%s - print portions of the Douay Rheims Bible\n",PNAME);
     } else if (lat){
     printf ("%s\n\n",LPROG);
     printf ("\n\n%s - print portions of the Latin Vulgate Bible\n",PNAME);
     }else{
	if (klv){
     printf ("%s\n\n",TPROG);
     printf ("\n\n%s - print portions of the KLV Bible\n",PNAME);
	}else{
     printf ("%s\n\n",PROG);
     printf ("\n\n%s - print portions of the WEB Bible\n",PNAME);
	}
   }
   }

   printf ("\n");
   printf ("Usage:\n");
   printf ("\n");
   printf ("\tbook [chapter [verse  [end-verse] ] ]\n");
   printf ("\t>filename - write output to filename\n");
   printf ("\t!command  - execute command\n");
   printf ("\tweb       - Switch to WEB translation\n");
   printf ("\tkjv       - Switch to KJV translation\n");
   printf ("\tklv       - Switch to Klingon Language Version\n");
   printf ("\tdrv       - Switch to DRV translation\n");
   printf ("\tlat       - Switch to Vulgate (Latin) translation\n");
   printf ("\t*quit     - end program\n");
   printf ("\t*wrap 99  - set wrap at column 99 (set to large number to turn off\n");
   printf ("\t?         - this summary\n");
   printf ("\t*books    - list books\n");
   printf ("\n");
   if (klv){
     printf ("\n\nKLBible : Klingon Language Bible Assistant for the Klingon Language Version Bible - %s.%s\nJoel Peter Anderson, %s\n",VERS,LVL,DATE);
   }
   if (kjv){
     printf ("\n\nKJBible Assistant for the King James Bible - %s.%s\nJoel Peter Anderson, %s\n",VERS,LVL,DATE);
   }else{
     if (drv){
       printf ("\n\nDRBible Assistant for the Douay Rheims Bible - %s.%s\nJoel Peter Anderson, %s\n",VERS,LVL,DATE);
     }else{
       printf ("\n\nWEBible Assistant for the World English Bible - %s.%s\nJoel Peter Anderson, %s\n",VERS,LVL,DATE);
     }
   }
 }
 
 /*
  *                      _ _        _   
  *   _ __ ___  __ _  __| | |___  _| |_ 
  *  | '__/ _ \/ _` |/ _` | __\ \/ / __|
  *  | | |  __/ (_| | (_| | |_ >  <| |_ 
  *  |_|  \___|\__,_|\__,_|\__/_/\_\\__|
  *                                   
  */
readtxt (FILE *file, int tbook, int tchap, int tverse, int tvend){
  FILE *BOOK;
  char btitle[2000];
  sprintf(btitle,"%s/web.dir/%d.web",FILEHOME,tbook);
  if (tbook){
    if (mnd) {
    fprintf (file,"Looking for %s \n",BOOKS[tbook]);
	sprintf(btitle,"%s/mnd.dir/%d.mnd",FILEHOME,tbook);
    }else if (lat) {
      fprintf (file,"Looking for %s \n",LBOOKS[tbook]);
	sprintf(btitle,"%s/vul.dir/%s.gbf",FILEHOME,LBOOKS[tbook]);
    } else if (kjv) {
      fprintf (file,"Looking for %s \n",KBOOKS[tbook]);
      sprintf(btitle,"%s/kjv.dir/%d.kjv",FILEHOME,tbook);
    } else if (klv) {
      fprintf (file,"Looking for %s \n",TBOOKS[tbook]);
      sprintf(btitle,"%s/klv.dir/%d.klv",FILEHOME,tbook);
    }else {
      if(drv){
	fprintf (file,"Looking for %s \n",DBOOKS[tbook]);
	sprintf(btitle,"%s/drv.dir/%s.gbf",FILEHOME,DBOOKS[tbook]);
      }else{
	fprintf (file,"Looking for %s \n",BOOKS[tbook]);
      }
    }
     
    BOOK=fopen (btitle,"r");

    while (fgets (input,2000,BOOK) ){
      sline(input);

      if ((tchap ==0) &&(tverse ==0)){
	fprintf (file,"%d:%-6d%s",curchap,curverse,tline);
      }else if ((tchap == curchap) &&(tverse ==0)){
	fprintf (file,"%d:%-6d%s",curchap,curverse,tline);
      }else if ((tchap == curchap) &&(tverse == curverse) && (tvend == 0)){
	fprintf (file,"%d:%-6d%s",curchap,curverse,tline);
      }else if ((tchap == curchap) &&(tverse <= curverse) && (tvend >= curverse)){
	fprintf (file,"%d:%-6d%s",curchap,curverse,tline);
      }

    }
  }
}
 
 /*****************************/
 main (int argc, char *argv[]){

   FILE *outf = (FILE *)0;
   int done = 0;
   int i;
   char *progn;  /*for program name*/
   int tbook, tchap, tverse, tvend;
   char xbook[300];
   char *filename;
   char *PNAME = basename(argv[0]);
   tbook = tchap = tverse = tvend = 0;

   if (strchr(PNAME,'w')||strchr(PNAME,'w')){
	 mnd = 0;
     lat = 0;
     drv = 0;
     kjv = 0;
     maxbooks = 66;
     progn = "WEBible";
     OPROG = PROG;
   } 
  if (strchr(PNAME,'a')||strchr(PNAME,'A')){
	 mnd = 0;
     lat = 1;
     drv = 0;
     kjv = 0;
     maxbooks = 78;
     progn = "LatBible";
     OPROG = LPROG;
     printf ("[Running %s ]\n",OPROG);
   }
   if (
       (strchr(PNAME,'m')||strchr(PNAME,'M'))&&
       (strchr(PNAME,'n')||strchr(PNAME,'N'))&&
       (strchr(PNAME,'d')||strchr(PNAME,'D'))) {
     mnd = 1;
     lat = 0;
     drv = 0;
     kjv = 0;
     maxbooks = 66;
     progn = "MNDBible";
     OPROG = MPROG;
   } 
   if (strchr(PNAME,'k')||strchr(PNAME,'K')){
     char *nchar;
     if (strchr(PNAME,'k'))       {
       nchar = strchr(PNAME,'k')+1;
     }else{
       nchar = strchr(PNAME,'K')+1;
     }
     
     if ((*nchar == 'l')||(*nchar == 'L'))
       {
	 mnd = 0;
	 kjv = 0;
	 klv = 1;
	 lat = 0;
	 drv = 0;
	 maxbooks = 66;
	 progn = "KLBible";
	 OPROG = TPROG;
	 printf ("[Running %s ]\n",OPROG);
       }else{
	 kjv = 1;
	 maxbooks = 81;
	 progn = "KJBible";
	 OPROG = KPROG;
       }
   }else{
   if (
       (strchr(PNAME,'D')||strchr(PNAME,'d'))&&
       (strchr(PNAME,'r')||strchr(PNAME,'R'))
       ){
     drv = 1;
     maxbooks = 73;
     progn = "DRBible";
     OPROG = DPROG;
   }else{
     maxbooks = 66;
     progn = "WEBible";
     OPROG = PROG;
   }
   }

   if (argc < 2) {
     usage2();

     while (!done){
       tbook = tchap = tverse = tvend = 0;
       printf ("\n%s> ",progn);
       cmd[0]='\0';
       xbook[0]='\0';
       gets(cmd);
       if (*cmd == '!'){
	 if (outf) {
	   fclose(outf);
	   outf = (FILE*)0;
	 }
	 cmd[0]= ' ';
	 system (cmd);
       }else  if (*cmd == '>'){
	 if (outf) {
	   fclose(outf);
	   outf = (FILE*)0;
	 }
	 cmd[0]= ' ';
	 filename = &cmd[1];
	 outf = fopen (filename,"w");
       }else  if (!strncasecmp(cmd,"lat",3)){
	 lat = 1;
	 klv = 0;
	 mnd = 0;
	 drv = 0;
	 kjv = 0;
	 maxbooks = 78;
	 progn = "LatBible";
	 OPROG = LPROG;
	 printf ("[Running %s ]\n",OPROG);
       }else  if (!strcasecmp(cmd,"kjv")){
	 kjv = 1;
	 mnd = 0;
	 lat = 0;
	 klv = 0;
	 drv = 0;
	 maxbooks = 81;
	 progn = "KJBible";
	 OPROG = KPROG;
	 printf ("[Running %s ]\n",OPROG);
        }else  if (!strcasecmp(cmd,"mnd")){
 	 kjv = 0;
 	 mnd = 1;
	 lat = 0;
	 klv = 0;
	 drv = 0;
	 maxbooks = 66;
	 progn = "MNDBible";
	 OPROG = MPROG;
	 printf ("[Running %s ]\n",OPROG);
       }else  if (!strcasecmp(cmd,"drv")){
	 kjv = 0;
	 lat = 0;
	 mnd = 0;
	 klv = 0;
	 drv = 1;
	 maxbooks = 73;
	 progn = "DRBible";
	 OPROG = DPROG;
	 printf ("[Running %s ]\n",OPROG);
       }else  if (!strcasecmp(cmd,"web")){
	 mnd = 0;
	 klv = 0;
	 kjv = 0;
	 lat = 0;
	 drv = 0;
	 maxbooks = 66;
	 progn = "WEBible";
	 OPROG = PROG;
	 printf ("[Running %s ]\n",OPROG);
       }else  if (!strcasecmp(cmd,"klv")){
	 kjv = 0;
	 mnd = 0;
	 klv = 1;
	 lat = 0;
	 drv = 0;
	 maxbooks = 66;
	 progn = "KLBible";
	 OPROG = TPROG;
	 printf ("[Running %s ]\n",OPROG);
       }else  if (*cmd == '?'){
	 usage2();
       }else  if (!strcasecmp(cmd,"*books")){
	 usage();
       }if (!strncasecmp(cmd,"*wrap",5)){
	   sscanf(cmd+5,"%d",&WRAPCOL);
	   printf ("\n\tWrap set to %d\n",WRAPCOL);
       }else  if (!strcasecmp(cmd,"*quit")){
	 done = 1;
	 printf ("\n\n%s - %s.%s\nJoel Peter Anderson, %s\n",OPROG,VERS,LVL,DATE);
	 printf ("Visit the UTA Project at \n\thttp://www.MrKlingon.org\n\n");
	 exit(0);
       } else{
	 sscanf(cmd,"%s %d %d %d",xbook,&tchap,&tverse,&tvend);
	 if (kjv){
	   tbook=iskbook(xbook);
	 }else{
	   if (lat){
	     tbook=islbook(xbook);
	   }else if (drv){
	     tbook=isdbook(xbook);
	   }else{
	     tbook=isbook(xbook);
	   }
	 }
	 if ((tbook<1) ||(tbook>maxbooks)){
	   sscanf(xbook,"%d",&tbook);
	 }
	 readtxt(stdout,tbook,tchap,tverse,tvend);
	 if (outf) { readtxt(outf,tbook,tchap,tverse,tvend); }
       }
     }
   }else{

     if (!strncmp("help",argv[1],4)){
       	 usage();
	 exit(0);
     }
       

     /* look for -w wrap param */
     {
       int a,b;
       for (a=1;a<argc;a++){
	 if(!strncmp ("-w",argv[a],2) || !strncmp ("-W",argv[a],2)){
	   sscanf(&argv[a][2],"%d",&WRAPCOL);
	   for (b=a;b<argc-1;b++){
	     argv[b]=argv[b+1];
	   }
	   argc --;
	 }
       }
     }
     if (!strcmp("SPLIT",argv[1])) {
       splitweb();
       exit(0);
     }
     tbook=isbook(argv[1]);
     if (lat){
       tbook=islbook(argv[1]);
     } else if (kjv){
       tbook=iskbook(argv[1]);
     }else if (drv){
	 tbook=isdbook(argv[1]);
     }

     if (argc>2) {sscanf(argv[2],"%d",&tchap);}
     if (argc>3) {sscanf(argv[3],"%d",&tverse);}
     if (argc>4) {sscanf(argv[4],"%d",&tvend);}

     if ((tbook<1) ||(tbook>maxbooks)){
       sscanf(argv[1],"%d",&tbook);
     }

     if ((tbook<1) ||(tbook>maxbooks)){
       usage();
       exit(0);
     }
     readtxt(stdout,tbook,tchap,tverse,tvend);
   }
 }
