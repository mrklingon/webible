#!/usr/bin/perl                                                                                                                                                                                                                                                                                                     
##############################                                                                                                                                                                                                                                                                                      
#  BBuild.pl                                                                                                                                                                                                                                                                                                        
#                                                                                                                                                                                                                                                                                                                   
# BBuild - build podcasts from Biblical texts                                                                                                                                                                                                                                                                       
#                                                                                                                                                                                                                                                                                                                   
#                                                                                                                                                                                                                                                                                                                   
#bible versions.                                                                                                                                                                                                                                                                                                    

if (!defined  ($ARGV[1])){
    printf "BBuild - build Bible podcasts.\n";
    printf "\n\nUsage: BBuild subject verse-list-file\n";
    printf "\n";
    exit;
}else{
    $SUBJ = $ARGV[0];
    $VERSES = $ARGV[1
];
    open VRS, "<$VERSES";
}

$MND = "mndbible";
$WEB = "webible";
$KLV = "klbible";

open KX,">$SUBJ-klingon-out.html";
open MX,">$SUBJ-mandoa-out.html";


while (<>){
    chomp;
    printf KX "<b>$_</b>\n";
    printf MX "<b>$_</b>\n";
    $inp = $_;
    $inp =~ s/:/ /g;
    $inp =~ s/\./ /g;



    printf KX "<h3>What is ......................? <p>Listen to what the Bible says, from xxxxxxxx.</h3><p>\n(click for podcast) <p>\n<center><i><b>\n";
    printf MX "<h3>What is ......................? <p>Listen to what the Bible says, from xxxxxxxx.</h3><p>\n(click for podcast) <p>\n<center><i><b>\n";
    open KBIB,  "$KLV $inp|bfilt|";
    open MBIB, "$MND $inp|bfilt|";
    open BIB,  "$WEB $inp|bfilt|";

    while (<KBIB>){
        printf KX $_;
    }
    while (<MBIB>){
        printf MX $_;
    }

    printf KX "</center></i></b>\n";
    printf MX "</center></i></b>\n";
    printf KX "<center><i>\n";
    printf MX "<center><i>\n";

    while (<BIB>){
        printf MX $_;
        printf KX $_;
    }

    printf MX "</center></i>\n<p><p><b>Listen to the Word, it reaches even to galaxies far, far away</b><p><hr>\n";
    printf KX "</center></i>\n<p><p><b>Listen to the Word, it helps us navigate the stars and beyond.</b><p><hr>\n";


}
