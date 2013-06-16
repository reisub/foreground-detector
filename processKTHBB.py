#!/usr/bin/env python3

actions = ["boxing","handclapping","handwaving","jogging","running","walking"]

bbInfo = {}

fh = open("KTHBoundingBoxInfo.txt","r")

for line in fh.readlines():
    if line.startswith("#") or line == "":
        continue
    elements = line.split()
    person = elements[0]
    scenario = elements[1]
    action = int(elements[2])
    sequence = elements[3]
    startFrame = int(elements[4])
    endFrame = int(elements[5])


    filename = "person%02i_%s_d%s_uncomp.avi" % (int(person), actions[action-1],scenario)
    totalFrames = endFrame-startFrame+1
    frameNumber = startFrame

    outputLines = []
    for i in range(6,6+totalFrames*4,4):

        ymin = int(elements[i])
        xmin = int(elements[i+1])
        ymax = int(elements[i+2])
        xmax = int(elements[i+3])

        # SPTG format (lower left, upper right)
        x1 = xmin
        y1 = ymax
        x2 = xmax
        y2 = ymin

        identifier = "{}{}{}{}".format(person,action,scenario,sequence)
        outputLine = "{} {} {} {} {}".format(frameNumber, x1, y1, x2, y2)
        frameNumber = frameNumber + 1
        outputLines.append(outputLine)

    if bbInfo.has_key(filename):
        bbInfo[filename].extend(outputLines)
    else:
        bbInfo[filename] = outputLines

for filename in bbInfo.keys():
    print(filename + ":")
    for frame in bbInfo[filename]:
        print(frame)
    print("#")
