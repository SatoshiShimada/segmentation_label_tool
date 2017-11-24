# Labeling tool

A generate tool of label for segmentation dataset.

## How to use

First, create file of list of image.
For example, `ls [Image Direcotry] > image_list`.  
Second, select the pixel using `./Labeling_tool`.
Generated pixel data file. `[Image name].txt`.  
Finaly, generate label image from pixel data file.
Using `Label_generator`. For example, `./Label_generator [label image name] < [pixel data file name]`.

## How to build
Execute follow command:

```shell
cd labeling_tool/
./build.sh
make
cd ../

cd label_generator/
make
cd ../
```

## Author
Satoshi Shimada

