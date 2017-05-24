from __future__ import print_function

import os
import parser as xmlParser

if __name__ == '__main__':
    date = '2011_09_26'
    drive = 64
    kittiDir = '/home/nghia/workplace/data/kitti/%s/%s_drive_%04d_sync/' % (date, date, drive)
    imageFolder = os.path.join(kittiDir, 'image_02/data')
    trackletFile = os.path.join(kittiDir, 'tracklet_labels.xml')
    trackletFolder = os.path.join(kittiDir, 'tracklets')
    tracklets = xmlParser.parseXML(trackletFile)

    nFrames = len(os.listdir(imageFolder))
    frames = [[] for _ in range(nFrames)]

    for tracklet in tracklets:
        firstFrame = tracklet.firstFrame
        for i, frame in enumerate(tracklet):
            # print(frame)
            # exit()
            frames[i+firstFrame].append((frame, tracklet.size, tracklet.objectType))

    if not os.path.exists(trackletFolder):
        os.makedirs(trackletFolder)

    minRot = 0
    maxRot = 0
    for x, frame in enumerate(frames):
        with open(os.path.join(trackletFolder, '%010d.txt' % x), 'w') as f:
            for y, v in enumerate(frame):
                (trans, rot, state), size, objectType = v
                iterpolate = (objectType, size[0], size[1], size[2], trans[0], trans[1], trans[2], rot[0], rot[1], rot[2], state)
                f.write('%s %f %f %f %f %f %f %f %f %f %d\n' % iterpolate)

                minRot = min((minRot, rot[2]))
                maxRot = max((maxRot, rot[2]))

    print(minRot, maxRot)