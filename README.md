# Archiver-RLE

This is a console program that allows you to encode and decode files using the RLE algorithm.

## Dependencies

### Make

Ubuntu:

```bash
sudo apt-get update
sudo apt install make
```
Windows:

[Guide](https://stackoverflow.com/questions/32127524/how-to-install-and-use-make-in-windows)

### gcc

Ubuntu:

```bash
sudo apt-get update
sudo apt install gcc
```

Windows:

[Guide](https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j)

## Usage

```bash
cd PATH/TO/DIR/archiver_RLE/
make build
```

You can run the ```program archiver_RLE.out```. 
First, enter the name of the input file, and then the output file. Then select the program mode (ZIP/UNZIP). Yoy need add files to workfolder.

### Make commands
- build
- start
- all
- clean
- clean_SMTH

### Debug

If you want enable debug mode, pass DEBUG_=1 as a flag. 
Then you will be able to view the logger dumps.
You can find logs in ```logout.log```.