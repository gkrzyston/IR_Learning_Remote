# EECS373_IR_Learning_Remote
Creating an IR Learning Remote for our EECS 373 Final Project.
## Organization
### Trunk:
Trunk should house the current IOC, main.c and other files which will be used to integrate each of our modules into.  Before merging into trunk, be sure that your module is compatible with the IOC and main.c currently present in this branch (or ensure that none of your changes adversely affect any other functionality) so we can avoid compatibility issues.
### Development Branches:
All code being developed for the project should find its way into the applicable dev/<module> branch when it is in a working state.  In-progress work which does not compile or work properly can be stored at dev/<module>/<your_name>.
### Command Line Interface:
The CLI is housed under dev/interface.  Do not push code that does not compile.  Temporary files to sync between computers can be stored on dev/interface/<your_name> if needed.
