#!/usr/local/bin/fish

set objs (eval "ls -1 obj")

for obj in $objs
  echo "Profiling with " $obj
  for i in (seq 5)
    eval "./m$argv[1] f obj/$obj"
  end
end
