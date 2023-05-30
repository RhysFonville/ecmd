<h1>ecmd</h1>

Should work on both linux and windows. The CmdPrompt example only works on windows.

To get repo on Linux:

```bash
git clone git@github.com:RhysFonville/ecmd.git path/to/directory

# I recommend that you do this to remove likely unneeded files. It seems very repetitive, I'm tired. I don't even know if it works.
cd path/to/directory
cd ecmd
mv ecmd ecmdtemp
mv ecmdtemp ..
cd ..
rm -r ecmd
mv ecmdtemp ecmd
```

To get repo on windows:

```bash
git clone git@github.com:RhysFonville/ecmd.git path/to/directory

# I recommend that you do this to remove likely unneeded files. It seems very repetitive, I'm tired. I don't even know if it works.
cd path/to/directory
cd ecmd
move ecmd ecmdtemp
move ecmdtemp ..
cd ..
del /s /q ecmd
move ecmdtemp ecmd
```
