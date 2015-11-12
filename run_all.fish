#!/usr/local/bin/fish

set methods 1 2 3 4 5 6

for val in $methods
  echo Running Method $val
  eval ./run_method.fish $val
end
