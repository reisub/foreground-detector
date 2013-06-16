#!/usr/bin/env ruby

if ARGV.length != 2
	abort "Usage: ruby #{$0} <file 1> <file 2>\n"
end

actions = ["walking", "running", "jogging", "boxing", "handclapping", "handwaving"]
scenarios = ["d1", "d2", "d3", "d4"]

# format: frame minX maxY maxX minY

# TODO
#
# store all bounding boxes in hash for key filename
# hash[filename] => hash[frame] => bounding box
#

#
# File.open('xxx.txt').each do |line|
#   print "#{line_num += 1} #{line}"
# end
#