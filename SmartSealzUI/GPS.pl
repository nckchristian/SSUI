#!/usr/bin/perl
#Cahlor Stedman
#Edited and Maintained by Brandon Mord
#Version 1.1
#GPS DATA GRAB using REGEX
#SmartSealz
#To run in Raspian -need to install LWP::Simple
#sudo perl -MCPAN -e'install "LWP::Simple"'

use LWP::Simple;
use strict;
my $thread = $$;
open(THREADFILE,"> DataFiles/perlThread.txt") or die ("No file");
printf THREADFILE ("%10d",$thread);
close THREADFILE;

my $webpage1 = 'http://192.168.10.1/getSituation';
my $source1 = get $webpage1;

$source1 =~ /"GPSLatitude":(\-?\d*\.\d*|\-?\d*)/;
my $GPSLat = $1, $source1 = $';
$source1 =~ /"GPSLongitude":(\-?\d*\.\d*|\-?\d*)/;
my $GPSLong = $1, $source1 = $';
$source1 =~ /"GPSTrueCourse":(\-?\d*\.\d*|\-?\d*)/;
my $GPSTrueCourse = $1, $source1 = $';
$source1 =~ /"GPSGroundSpeed":(\-?\d*\.\d*|\-?\d*)/;
my $GPSGroundSpeed = $1, $source1 = $';
$source1 =~ /"BaroPressureAltitude":(\-?\d*\.\d*|\-?\d*)/;
my $BaroPressureAltitude = $1, $source1 = $';
$source1 =~ /"AHRSPitch":(\-?\d*\.\d*|\-?\d*)/;
my $AHRSPitch = $1, $source1 = $';
$source1 =~ /"AHRSRoll":(\-?\d*\.\d*|\-?\d*)/;
my $AHRSRoll = $1, $source1 = $';
$source1 =~ /"AHRSGyroHeading":(\-?\d*\.\d*|\-?\d*)/;
my $AHRSGyroHeading = $1, $source1 = $';
$source1 =~ /"AHRSMagHeading":(\-?\d*\.\d*|\-?\d*)/;
my $AHRSMagHeading = $1, $source1 = $';

open (OUTFILE, "> DataFiles/gpsdata.txt") or die ("cannot write to designated file");
printf OUTFILE ("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f", $GPSLat, $GPSLong, $GPSTrueCourse, $GPSGroundSpeed, $BaroPressureAltitude, $AHRSPitch, $AHRSRoll, $AHRSGyroHeading, $AHRSMagHeading);
close OUTFILE;
