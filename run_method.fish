#!/usr/local/bin/fish

set seeds 3 7 11 13 17 19 23

for seed in $seeds
  eval ./m$argv[1] $seed
end
