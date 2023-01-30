# HTML Files

This folder only contains developing resources files, and not needed to copy to the dest server when deploy. CMakeList.txt will package all required resources files to final binary, for more informations, please see /controllers/readme.md.

这个目录中的文件仅在开发时使用，且不应该在此处编写HTML文件（而应该使用xc::processor::template中提供的构造方法，写死，写死是好事，我不能理解为什么部署一个系统需要那么多的*配置文件*和*资源文件*）。CMakeList.txt会在编译时将需要的文件生成C++代码放到Controller目录中，具体请参考/controllers/readme.md。