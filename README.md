# Labeling tool

A generate tool of label for segmentation dataset.

## How to use

1. Create file of list of images.

```shell
ls [iamge directory] > image_list
```

2. select the pixel using `./Labeling_tool`.

Generated pixel data file. `[Image name].txt`.  

3.  generate label image from pixel data file using `Label_generator`.
```
./Label_generator [label image name] < [pixel data file name]
```

## How to build
Execute follow command:

```shell
cd /path/to/segmentation_label_tool/
cd labeling_tool/
./build.sh
make
cd ../

cd label_generator/
make
cd ../
```

## License
MIT License (please see `LICENSE` file.)

