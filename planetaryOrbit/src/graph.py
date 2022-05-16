import matplotlib.pyplot as plt


f = open("../bin/simulation_data.txt", "r")
real_paths = {}
kepler_paths = {}
cowell_paths = {}
timestep = 0
scale = 0

# extract data
for line in f.readlines():
    if line.startswith("Timestep: "):
        # extract timestep data
        timestep = float(line.split()[1])
    elif line.startswith("xyz scale: "):
        # extract scale data
        scale = int(line.split("^")[1].split("k")[0])
    elif line[0].startswith("["):
        # extract position data
        name, data = line.strip()[1:].split("]")
        type, *pos = data.split()
        pos = tuple(map(float, pos))
        if type == "REAL":
            if name in real_paths.keys():
                real_paths[name].append(pos)
            else:
                real_paths[name] = [pos]
        elif type == "KEPLER":
            if name in kepler_paths.keys():
                kepler_paths[name].append(pos)
            else:
                kepler_paths[name] = [pos]
        elif type == "COWELL":
            if name in cowell_paths.keys():
                cowell_paths[name].append(pos)
            else:
                cowell_paths[name] = [pos]
                
f.close()

def get_distances(real_paths, simulated_paths):
    # compute distances between simulated datapoints and real datapoints
    dists = {}

    # initialize storage
    for name in real_paths.keys():
        dists[name] = []

    for name in real_paths.keys():
        # for each pair of positions, store the distance between them
        for index, pos1 in enumerate(real_paths[name]):
            pos2 = simulated_paths[name][index]
            # calculate distance
            dist_squared = 0
            for xyz in range(3):
                dist_squared += (pos1[xyz] - pos2[xyz])**2
            dist = round(dist_squared**0.5, 3) # the data is to 3d.p. so round the distance to 3d.p.
            dists[name].append(dist)
    return dists
            
kepler_dists = get_distances(real_paths, kepler_paths)
cowell_dists = get_distances(real_paths, cowell_paths)

# x axis time values
time_values = range(len(list(real_paths.values())[0]))

for name in real_paths.keys():
    plt.plot(time_values, kepler_dists[name], label=name)
    #plt.plot(time_values, cowell_dists[name], label=name)

plt.xlabel("simulation time elapsed /" + str(timestep/3600) +"hrs")
plt.ylabel("distance to real body /10^" + str(scale) + "km")
plt.legend()
plt.show()
