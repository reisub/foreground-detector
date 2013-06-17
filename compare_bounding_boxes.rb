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

def fill_hash_from_file(input_file, output_hash)
	text = IO.read(input_file)
	videos = text.split("#\n")

	puts videos.size

	videos.each do |video|
		lines = video.split("\n")
		filename = lines.shift
		if !filename
			next
		end
		filename.chomp!(":")
		frames = {}
		lines.each do |line|
			values = line.split(" ")
			values.collect! do |value|
				value.to_i
			end
			frameNr = values.shift()
			frames[frameNr] = values
		end
		output_hash[filename] = frames
	end
end

def calculate_surface(values)
	x1 = values[0]
	x2 = values[2]
	y1 = values[3]
	y2 = values[1]

	a = x2 - x1
	b = y2 - y1

	return a*b
end

def calculate_intersection(values1, values2)
	x1 = [values1[0], values2[0]].max
	x2 = [values1[2], values2[2]].min
	y1 = [values1[3], values2[3]].max
	y2 = [values1[1], values2[1]].min

	s1 = calculate_surface(values1)
	s2 = calculate_surface(values2)
	inter = calculate_surface([x1, y2, x2, y1])
	if inter < 0.0
		return 0
	end

	return inter/([s1, s2].max).to_f
end

first = {}
second = {}

fill_hash_from_file(ARGV[0], first)
fill_hash_from_file(ARGV[1], second)

first.each_key do |filename|
	frames = first[filename]
	frames.each_key do |frameNr|
		if second[filename].key?(frameNr)
			puts calculate_intersection(frames[frameNr], second[filename][frameNr])
		end
	end
end