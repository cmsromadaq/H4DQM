#!/bin/bash

function post_install ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> POST INSTALL"
	return 
    fi
    # POST INSTALL
    /usr/sbin/lcm --configure ntpd afscl
    cat /etc/sysconfig/yum-autoupdate | sed -e 's:YUMUPDATE=1:YUMUPDATE=0:g' > /etc/sysconfig/yum-autoupdate.tmp
    mv /etc/sysconfig/yum-autoupdate.tmp /etc/sysconfig/yum-autoupdate
    /sbin/chkconfig --add yum-autoupdate
    /sbin/service yum-autoupdate start
    /usr/sbin/lcm --configure srvtab
    /usr/sbin/lcm --configure krb5clt sendmail ntpd chkconfig ocsagent ssh
    cern-config-users --setup-sudo-access --setup-user-accounts --setup-printers additional-sudoers
}

function root ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> ROOT"
	return 
    fi
    # install pkgs
    yum install -y sl-release-scl gsl-devel
    yum install -y freenx nxagent nxauth nxproxy xorg-x11-apps screen
    yum install -y git make binutils libX11-devel libXpm-devel libXft-devel libXext-devel
    yum install -y devtoolset-3-gcc-* devtoolset-3-gdb devtoolset-3-binutils devtoolset-3-elfutils python27* 
    source /opt/rh/python27/enable
    source /opt/rh/devtoolset-3/enable
    echo "source /opt/rh/devtoolset-3/enable" > /etc/profile.d/zzzz_dvts-3.sh
    echo "source /opt/rh/python27/enable" > /etc/profile.d/zzzz_py27.sh
    echo "source /opt/root/bin/thisroot.sh" > /etc/profile.d/root6.sh

    cd /tmp
    wget https://root.cern.ch/download/root_v6.04.14.source.tar.gz
    cd /opt
    tar -xzf /tmp/root_v6.04.14.source.tar.gz; mv root-6.04.14 root
    cd root/
    ./configure --prefix=/usr --enable-genvector --enable-mathmore --enable-minuit2 --enable-opengl --enable-python --enable-reflex --enable-roofit --enable-xrootd
    make -j 3
    make install
}

function zeromq ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> ZEROMQ"
	return 
    fi
    # get ZEROMQ
    yum install -y libsodium libsodium-devel
    cd /tmp/
    curl -o zeromq-4.1.4.tar.gz https://cernbox.cern.ch/index.php/s/SBUmAYCdRv2I2rK/download 
    cd /opt
    tar -xzf /tmp/zeromq-4.1.4.tar.gz 
    cd zeromq-4.1.4
    ./configure --prefix=/usr
    make -j 3
    make install
    cd /tmp
    wget https://raw.githubusercontent.com/zeromq/cppzmq/master/zmq.hpp
    cp zmq.hpp /usr/include/
    ldconfig
    easy_install pyzmq
}

function caenlib ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> CAENLIB"
	return 
    fi
    # get CAEN lib
    cd /tmp
    curl -o CAENComm-1.2-build20140211.tgz https://cernbox.cern.ch/index.php/s/wHT5e5aJOPencyy/download
    curl -o CAENDigitizer_2.6.8.tgz https://cernbox.cern.ch/index.php/s/Z5uzpOJehmQphZA/download
    curl -o CAENUpgrader-1.5.2-build20120724.tgz https://cernbox.cern.ch/index.php/s/sONBXV0iLl8pf3r/download
    curl -o CAENVMELib-2.50.tgz https://cernbox.cern.ch/index.php/s/pTKdUwIZsMayRKr/download
    cd /opt
    tar -xzf /tmp/CAENComm-1.2-build20140211.tgz 
    tar -xzf /tmp/CAENDigitizer_2.6.8.tgz 
    tar -xzf /tmp/CAENUpgrader-1.5.2-build20120724.tgz 
    tar -xzf /tmp/CAENVMELib-2.50.tgz

    cd CAENVMELib-2.50/lib/
    ./install_x64
    cd ../../CAENComm-1.2/lib/
    ./install_x64
    cd ../../CAENDigitizer_2.6.8/
    ./install_64
    cd ../CAENUpgrader-1.5.2
    ./configure
    make -j 4
    make install
    ldconfig
}

function a2818_driver()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> A2818"
	return 
    fi
    # get CAEN lib
    cd /tmp
    curl -o A2818Drv-1.19.tgz https://cernbox.cern.ch/index.php/s/sB4GVBXEWm36xwi/download
    cd /opt
    tar -xzf /tmp/A2818Drv-1.19.tgz
    cd A2818Drv-1.19
    make
    sh a2818_load
}

function arduino ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> ARDUINO"
	return 
    fi
    # get arduino lib
    cd /tmp
    curl -o arduino-1.6.8-linux64.tar.xz https://cernbox.cern.ch/index.php/s/6ZF6oZxvRyfKkur/download
    cd /opt
    tar -xf /tmp/arduino-1.6.8-linux64.tar.xz
    
}

function useradd ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> USERADD"
	return 
    fi
    # set cmsdaq user
    /usr/sbin/useradd -d /home/cmsdaq -U -G dialout -p '$1$LVNQmNAw$ySPQzDdy3cG5KogZL.5Qf/' -m cmsdaq
    su cmsdaq <<EOF
    mkdir -p /home/cmsdaq/.ssh
    echo 'ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEAkrNSeBxzuZlyTp0ni28eyU2EogwedJCZuZ/e+6stkM1iwXAdPzcbm6BK1VAdzCOlqGzu8RueDit6KNvWzCzqLeJcIU5K46OgWcNZDuf25Qxe0rNbxrYP6JCpLojaPdXDSGTrCxza+7VJTUN680ew39GFzOBMy1gV5G9VPdeX37dwutCeQ0JN3hb3MLbHXILC0ZqjeU6HuuaT+Ev3/B86lTCICUy/bEL6UyhQrco75Oo7zeQkPt3dKNcFESiNJHgzuwvoGtkjud04uEYdldcZWcQ3XQTRC3bFblei/TFiyxmoAtphO7b5+QCoaAHuoTkxFqqcmZPbslkWazcviajAjw== cmsdaq@myhost' > /home/cmsdaq/.ssh/id_rsa.pub
    cat << EOL > /home/cmsdaq/.k5login
meridian@CERN.CH
spigazzi@CERN.CH
michelif@CERN.CH
shervin@CERN.CH
bmarzocc@CERN.CH
amartell@CERN.CH
EOL
EOF
}

function h4sw ()
{
    # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> H4SW"
	return 
    fi
    # install H4DAQ H4DQM H4GUI H4Analysis as cmsdaq
    yum install mysql-connector-python.noarch
    cd /home/cmsdaq/.ssh/
    curl -o id_rsa https://cernbox.cern.ch/index.php/s/PkRK1Dkb2ibZUHo/download
    su cmsdaq <<EOF
    mkdir -p /home/cmsdaq/DAQ
    cd /home/cmsdaq/DAQ
    git clone -b master git@github.com:cmsromadaq/H4DAQ.git
    git clone -b master git@github.com:cmsromadaq/H4DQM.git
    git clone -b master git@github.com:cmsromadaq/H4GUI.git
    git clone --recursive git@github.com:simonepigazzini/H4Analysis.git
    cd /home/cmsdaq/DAQ/H4DAQ
    ./configure.py
    make -j 4
    cd /home/cmsdaq/DAQ/H4DQM
    make -j 4
    cd /home/cmsdaq/DAQ/H4Analysis
    make
    cd /home/cmsdaq/DAQ/H4GUI
    echo "db...cmsdaq" > /home/cmsdaq/DAQ/H4GUI/cmsdaq_mysql.passwd 
EOF
}

function mysql_db()
{
   # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> MYSQL"
	return 
    fi
    yum install -y mysql-server mysql
    /etc/init.d/mysqld start
    mysqladmin -u root password db...h4daq
    /sbin/chkconfig mysqld --levels 35 on
    curl -o '/tmp/rundb_v2.mysql' https://cernbox.cern.ch/index.php/s/qMkuIN7WEDMuEOD/download
    /usr/bin/mysql -u root -p'db...h4daq' < /tmp/rundb_v2.mysql
}

function web_server()
{
   # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> HTTPD"
	return 
    fi

    yum install -y httpd
    service httpd start
    /sbin/chkconfig httpd --levels 35 on
    yum install -y php php-mysql
    cat <<EOF > /etc/httpd/conf.d/dqmsite.conf 
<Directory "/data/public_DQM_plots">
    AllowOverride None
    Options +Indexes
    IndexOptions +FancyIndexing
    Order allow,deny
    Allow from all
</Directory>
EOF
}

function data_disk()
{
   # DRYRUN
    if [ ${1} == 1 ]; then
	echo ">> DATA DISK"
	return 
    fi
    chown -R cmsdaq.cmsdaq /data/
    su cmsdaq <<EOF
    mkdir -p /data
    mkdir -p /data/raw/EB/
    mkdir -p /data/raw/DataTree/
    mkdir -p /data/DQM/
EOF
}
### MAIN ###

TEMP=`getopt -o d --long dryrun,useradd,arduino,caenlib,a2818_driver,zeromq,root,post_install,h4sw,mysql_db,web_server,data_disk -n 'install_h4daq.sh' -- "$@"`

if [ $? != 0 ] ; then echo "Options are wrong..." >&2 ; exit 1 ; fi

# Note the quotes around `$TEMP': they are essential!
eval set -- "$TEMP"

all=1
dryrun=0

while true; do
    case "$1" in
	-d | --dryrun ) dryrun=1; shift;;
	--post_install ) post_install=1; all=0; shift;;
	--root ) root=1; all=0; shift;;
	--zeromq ) zeromq=1; all=0; shift;;
	--caenlib ) caenlib=1; all=0; shift;;
	--a2818_driver ) a2818_driver=1; all=0; shift;;
	--arduino ) arduino=1; all=0; shift;;
	--useradd ) useradd=1; all=0; shift;;
	--h4sw ) h4sw=1; all=0; shift;;
	--mysql_db ) mysql_db=1; all=0; shift;;
	--web_server ) web_server=1; all=0; shift;;
	--data_disk ) data_disk=1; all=0; shift;;
	-- ) shift; break ;;
	* ) break ;;
    esac
done

for opt in post_install root zeromq caenlib a2818_driver arduino useradd mysql_db web_server data_disk; 
do    
    eval var=\$$opt
    if [ "$all" == 1 ] || [ "$var" == 1 ]; then
    	$opt $dryrun
    fi
done

exit 0
