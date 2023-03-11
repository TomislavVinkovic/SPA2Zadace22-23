import re

linksFile = open("githubLinks.txt", "r")
namesFile = open("githubNames.txt", "w")

L = linksFile.readlines()
names = []
for line in L:
    line = re.sub("https://github.com/", "", line)
    line = re.sub("/SPA2Zadace22-23(\.git)?", "", line)
    names.append(line)

names[-1] += '\n'
namesFile.writelines(names)

linksFile.close()
namesFile.close()