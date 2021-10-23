from os import listdir, path, write
import imageio

def main():
    file_dir = path.join("C:/Projects/C++/Maze_Runner/images")
    images = []
    gif_path = path.join(file_dir,"gif.gif")
    writer = imageio.get_writer(gif_path,mode='I',duration=0.05)
    files = [path.join(file_dir, fi) for fi in listdir(file_dir)]
    files.sort(key=lambda f: int(filter(str.isdigit, f)))
    for f in files:
        print(f.title())
        image = imageio.imread(path.join(file_dir, f))
        writer.append_data(image)
    files.sort(key=path.getmtime,reverse=True)
    for f in files:
        print(f.title())
        image = imageio.imread(path.join(file_dir, f))
        writer.append_data(image)
    writer.close()
        

if __name__ == "__main__":
    main()
