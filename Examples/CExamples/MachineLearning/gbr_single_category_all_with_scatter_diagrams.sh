#!/usr/bin/env bash

# Build and run with single category

# Compile sources on first comparison
./gbr_single_category.sh -C -p -0 Machine1 -1 Machine2 -s
echo ""

# Process datasets
./gbr_single_category.sh    -p -0 Machine3 -1 Machine4 -s
echo ""
./gbr_single_category.sh    -p -0 Machine1 -1 Machine3 -s
echo ""
./gbr_single_category.sh    -p -0 Machine1 -1 Machine4 -s
echo ""
./gbr_single_category.sh    -p -0 Machine2 -1 Machine3 -s
echo ""
./gbr_single_category.sh    -p -0 Machine2 -1 Machine4 -s
echo ""
./gbr_single_category.sh    -p -0 Machine3 -1 Machine4 -s
echo ""
