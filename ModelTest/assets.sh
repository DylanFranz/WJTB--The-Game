#!/bin/sh

NITRO_ENGINE=$DEVKITPRO/nitro-engine
ASSETS=assets
TOOLS=$NITRO_ENGINE/tools
OBJ2DL=$TOOLS/obj2dl/obj2dl.py
IMG2DS=$TOOLS/img2ds/img2ds.py

mkdir -p data

python3 $OBJ2DL \
    --input $ASSETS/object.obj \
    --output data/object.bin \
    --texture 32 32

python3 $IMG2DS \
    --input $ASSETS/texture.png \
    --name texture \
    --output data \
    --format A1RGB5
