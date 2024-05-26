import matplotlib.pyplot as plt


def plot(vertex_path, face_path):
    i = []
    x = []
    y = []
    with open(vertex_path) as f:
        for line in f:
            lsplit = line.split(" ")
            i.append(int(lsplit[0]))
            x.append(float(lsplit[1]))
            y.append(float(lsplit[2]))

    plt.scatter(x, y,marker="1")

    faces = []
    with open(face_path) as ff:
        for line in ff:
            lsplit = line.split(" ")
            faces.append([int(lsplit[0]), int(lsplit[1]), int(lsplit[2])])

    lines = []
    for face in faces:
        one = face[0]
        two = face[1]
        three = face[2]
        lines.append([[x[one], x[two]], [y[one], y[two]]])
        lines.append([[x[one], x[three]], [y[one], y[three]]])
        lines.append([[x[two], x[three]], [y[two], y[three]]])

    for line in lines:
        xl = line[0]
        yl = line[1]
        plt.plot(xl, yl)

    plt.show()


if __name__ == '__main__':
    plot("Vertices.txt", "Faces.txt")
    plot("Vertices_Refined.txt", "Faces_refined.txt")

