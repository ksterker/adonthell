#!/bin/sh

APP=$1

# -- strip path from application name
appname=`echo "$APP" | sed 's/.*\///'`

# -- bundle name starts with capital
first=`echo ${appname:0:1} | tr 'a-z' 'A-Z'`
bundle=$first${appname:1}.app
fwdir=$bundle/Contents/Frameworks
resdir=$bundle/Contents/Resources/games/$first${appname:1}
plugindir=$bundle/Contents/PlugIns

echo "Creating $bundle"

# -- cleanup existing bundle
rm -rf $bundle

# -- create bundle structure
mkdir -p $bundle/Contents/MacOS
mkdir -p $fwdir
mkdir -p $resdir
mkdir -p $plugindir

# -- populate bundle
echo "APPL????" > $bundle/Contents/PkgInfo
cat > $bundle/Contents/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
        <key>CFBundleDevelopmentRegion</key>
        <string>English</string>
        <key>CFBundleExecutable</key>
        <string>$appname</string>
        <key>CFBundleGetInfoString</key>
        <string>Adonthell - Map Engine Demo</string>
        <key>CFBundleIconFile</key>
        <string>$appname</string>
        <key>CFBundleIdentifier</key>
        <string>com.linuxgames.adonthell</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>6.0</string>
        <key>CFBundleName</key>
        <string>Worldtest</string>
        <key>CFBundlePackageType</key>
        <string>APPL</string>
        <key>CFBundleShortVersionString</key>
        <string>0.4.0</string>
        <key>CFBundleSignature</key>
        <string>????</string>
        <key>CFBundleVersion</key>
        <string>0.4.0 Alpha 2</string>
        <key>NSMainNibFile</key>
        <string>MainMenu</string>
        <key>NSPrincipalClass</key>
        <string>NSApplication</string>
</dict>
</plist>
EOF

# -- copy application
cp $APP $bundle/Contents/MacOS

# -- copy backend libraries
cp -r /usr/local/lib/adonthell/* $plugindir

function copyLibs
{
    # -- find all non-standard shared libraries used by app
    for i in `otool -L $1 | awk '{ print $1 }' | grep usr/local` ; do

        # -- strip path from library name
        libname=`echo "$i" | sed 's/.*\///'`

        # -- copy them to the bundle framework directory, if not there yet
        if [ ! -f $fwdir/$libname ] ; then
            echo "Adding $libname to $fwdir"
            cp $i $fwdir
        fi
    done
}

# -- copy shared libraries used by application and backend modules
copyLibs $APP
for i in `find $plugindir -name *.so` ; do 
    copyLibs "$i"
done

function relocate
{
    local relpath="@executable_path/../Frameworks"
    local basename=`echo "$1" | sed 's/.*\///'`
    echo "Relocating $basename"

    # -- update location of referenced libraries
    for i in `otool -L $1 | awk '{ print $1 }' | grep usr/local` ; do

        libname=`echo "$i" | sed 's/.*\///'`

        if [ -f $fwdir/$libname ] ; then
            install_name_tool -change $i $relpath/$libname $1
        fi
    done

    # -- update location of shared libraries
    case $basename in
        *.dylib | *.so) 
            install_name_tool -id $relpath/$basename $1 

            # -- remove debugging information
            strip -S $1
        ;;
        *)
            strip -u -r $1
        ;;
    esac
}

# -- fix rpath
relocate $bundle/Contents/MacOS/$appname
for i in `find $fwdir -name *.dylib` ; do 
    relocate "$i"
done
for i in `find $plugindir -name *.so` ; do 
    relocate "$i"
done
