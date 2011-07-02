#!/usr/bin/perl
$version=`git describe --tags --always`;

require "./autobuild-targets.pm";

my $verbose;
if($ARGV[0] eq "-v") {
    $verbose =1;
    shift @ARGV;
}

my $mutilcore;
if($ARGV[0] eq "-j") {
    $mutilcore =1;
    shift @ARGV;
}

my $doonly;
if($ARGV[0]) {
    $doonly = $ARGV[0];
    print "Will only build $doonly\n" if($verbose);
}


$rev = `git describe --tags --always`;
chomp $rev;
print "Rockbox Rev: $rev\n\n" if($verbose);

# made once for all targets
sub runone {
    my ($dir, $confnum, $extra)=@_;
    my $a;

    if($doonly && ($doonly ne $dir)) {
        return;
    }

    mkdir "build-$dir";
    chdir "build-$dir";
    print "\nBuilding in build-$dir" if($verbose);

    $a = buildit($dir, $confnum, $extra);

    chdir "..";

    my $o="build-$dir/rockbox";
    if (-f $o) {
        my $newo="output/rockbox-$dir-$version";
        
        `mkdir -p output`;
        `mv $o.zip $newo.zip`;
    }

    print "Removing all contents in build-$dir\n" if($verbose);
    system("rm -rf build-$dir");

    return $a;
};

sub buildit {
    my ($target, $confnum, $extra)=@_;

    `rm -rf * >/dev/null 2>&1`;

    my $ram = $extra ? $extra : -1;
    my $c = "../tools/configure --type=n --target=$confnum --ram=$ram";

    print "\nCompiling $target...\n" if($verbose);
    print "C: $c\n" if($verbose);
    `$c`;

    print "Runing 'make'\n" if($verbose);
    print "Using \'-j\' for make to enable mutil-core support" if($verbose && $mutilcore);
    `make -j VERSION=$version` if($mutilcore);
    `make    VERSION=$version` if(!$mutilcore);

    print "Run 'make zip'\n" if($verbose);
    `make zip`;
}

# run make in tools first to make sure they're up-to-date
print "cd tools && make\n" if($verbose);
`(cd tools && make   ) >/dev/null` if($mutilcore);
`(cd tools && make -j) >/dev/null` if(!$mutilcore);

for my $b (&usablebuilds) {
    my $configname = $builds{$b}{configname} ? $builds{$b}{configname} : $b;
    runone($b, $configname, $builds{$b}{ram});
}

