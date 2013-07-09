#!/usr/bin/env ruby

if ARGV.length != 2
	abort "Usage: ruby #{$0} <file 1> <file 2>\n"
end

actions = ["walking", "running", "jogging", "boxing", "handclapping", "handwaving"]
scenarios = ["d1", "d2", "d3", "d4"]

def fill_hash_from_file(input_file, output_hash)
	text = IO.read(input_file)
	videos = text.split("#\n")

	videos.each do |video|
		lines = video.split("\n")
		filename = lines.shift
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

# format: frame minX maxY maxX minY

def calculate_surface(values)
	x1 = values[0]
	x2 = values[2]
	y1 = values[3]
	y2 = values[1]

	a = x2 - x1
	b = y2 - y1

	if a < 0 or b < 0
		return 0
	end

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

	return inter/([s1, s2].max).to_f
end

first = {}
second = {}

fill_hash_from_file(ARGV[0], first)
fill_hash_from_file(ARGV[1], second)

inter_actions = {}
inter_scenarios = {}

actions.each do |action|
	inter_actions[action] = []
end

scenarios.each do |scenario|
	inter_scenarios[scenario] = []
end

first.each_key do |filename|
	frames = first[filename]
	if !second.key?(filename)
		next
	end
	frames.each_key do |frameNr|
		if second[filename].key?(frameNr)
			arr = filename.split("_")
			act, scen = arr[1], arr[2]
			inter = calculate_intersection(frames[frameNr], second[filename][frameNr])
			inter_actions[act].push(inter)
			inter_scenarios[scen].push(inter)
			if inter < 0.1
				puts filename + " " + frameNr.to_s + " " + inter.to_s
			end
		end
	end
end

puts "Scenarios:"
inter_scenarios.each_key do |scenario|
	sum = 0
	inter_scenarios[scenario].each do |value|
		sum += value
	end
	puts scenario + ": " + (sum/inter_scenarios[scenario].size).to_s
end

all = []

puts

puts "Actions:"
inter_actions.each_key do |action|
	all += inter_actions[action]
	sum = 0
	inter_actions[action].each do |value|
		sum += value
	end
	puts action + ": " + (sum/inter_actions[action].size).to_s
end

sum = 0
all.each do |value|
	sum += value
end
avg = sum/all.size

puts
puts "Min: " + all.min.to_s
puts "Max: " + all.max.to_s
puts "Avg: " + avg.to_s
puts "Median: " + all.sort[all.size/2].to_s