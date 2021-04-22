# Test Kernel Module
This is a test kernel module

## Team



1. [Iván Chavero](https://github.com/imcsk8) *Master Jedi*
1. [Daniel López](https://github.com/danielcosmico) *Padwan*
2. [Hugo Edibray Becerra Gandara
](https://github.com/Edibraaay) *Padwan*
3. [Juan Daniel Villegas Terrazas](https://github.com/JuanDanielVillegas) *Padwan*
4. [Ana Marina Ortiz Ruiz ](https://github.com/MarinaOrtiz328) *Padwan*
5. [LUIS ENRIQUE FERNANDEZ REZA](https://github.com/lfernandez17) *Padwan*
6. [Ernesto Gutierrez Ortega](https://github.com/ern-gt) *Padwan*


# Module building

## Build dependencies

On a Fedora system install the `Development Tools`  and the `C Development Tools and Libraries`.

```
~$ sudo dnf groupinstall -y 'Development Tools' 'C Development Tools and Libraries'
~$ sudo dnf install -y kernel-headers
```

## Build module

**Enter the module directory**

```
~$ cd test_kernel_module/kernel_module
```

The Makefile defines how to build the module, execute the `make` command to compile the module.

```
kernel_module $ make
```

## Load module

Load the module using the `insmod` command.

```
~$ sudo insmod super_module.ko
```

### Check if the module has been loaded

```
~$ journalctl -f | grep kernel
```

## Check module major number

The major number is needed for creating the device that is used to interact with the module
```
~$ journalctl  | grep mknod
```


## Remove module

Remove the module using the `rmmod` command

```
~$ sudo rmmod super_module
```

## Create the module device

This module registers a device major number but it does not create the device. Create the `/dev/super_module` device manually.

```
~$ sudo mknod /dev/super_module c <device_major_number> 0
```

### Test the module

```
~$ cat /dev/super_module
```


### Git Basic commands

**Clone this repository**

```
~$ git clone git@github.com:Sistemas-Operativos-I-UACH/test_kernel_module.git
```

**Use a branch other than *main***

```
~$ git checkout *yourbranch*
```

**Add a file to next commit**

Add the `README.md` file to the commit:

```
~$ git add README.md
```

**Commit a the changes**
```
~$ git commit -m 'commit message'
```

or to open your favorite editor and add a more extense commit message:

```
~$ git commit
```

**Push changes to our public github repository**

This team is using github.com as the public repository and the *Master Jedi* repository as the main one.

```
~$ git  push
```

**Get and integrate the latest changes of the current branch from the public git repository**

```
~$ git pull
```

**Get all changes in all the branches**

```
~$ git fetch --all
```

Once you get the changes you must pull them to integrate them in the branch

```
~$ git checkout your_branch
~$ git pull
```


***Be happy***

# References

* https://education.github.com/git-cheat-sheet-education.pdf
* https://code.visualstudio.com/docs/editor/versioncontrol
* https://www.thegeekstuff.com/2013/07/write-linux-kernel-module/
