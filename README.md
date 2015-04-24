# segment
Updates to Jud Harward and Curtis Woodcock's segmentation algorithm.

> Woodcock, C and VJ Harward. 1992. Nested-hierarchical scene models and image segmentation. International Journal of Remote Sensing 13 (16): 3167-3187. <http://dx.doi.org/10.1080/01431169208904109>

## Roadmap

- [x] Extract segmentation algorithm from larger IPW library
    * Only import IPW components needed for segmentation
    * Working Makefile
    * Separate `segment` source from IPW dependencies
- [ ] Create test suite for validation of progress
    * Validate against `segment` on BU cluster
- [ ] Replace IPW IO with newly written IO using GDAL
- [ ] Toggle on/off writing of region map during segmentation passes
    * I'm not sure we care so much about tracking regionmap progress anymore
    * Would speed up program if we removed IO
- [ ] Upgrade to `int` datatypes from `uchar_t`
- [ ] Create Python friendly binding to core segmentation algorithm
    * Would be *VERY* nice to be able to call segmentation program on NumPy arrays
- [ ] Port other utilities useful for image segmentation from IPW

### Reference
- http://www.gdal.org/gdal_tutorial.html
- http://www.gdal.org/gdalinfo_8c.html
- https://github.com/rouault/gdal/blob/trunk/gdal/apps/gdal_translate.cpp
