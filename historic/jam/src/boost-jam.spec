Name: boost-jam
Version: 3.1.2
Summary: Build tool
Release: 1
Source: boost-jam-%{version}.src.tar.gz

Copyright: GPL
Group: Development/Tools
URL: http://www.boost.org
Packager: Vladimir Prus <ghost@cs.msu.su>
BuildRoot: /var/tmp/boost-jam-%{version}.root

%description
Boost Jam is a build tool based on FTJam, which in turn is based on 
Perforce Jam. It contains significant improvements made to facilitate
its use in the Boost Build System, but should be backward compatible 
with Perforce Jam.

Authors:
	Perforce Jam : Cristopher Seiwald
	FT Jam       : David Turner
	Boost Jam    : David Abrahams

%prep
%setup -n boost-jam-%{version}

%build
make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
mkdir -p $RPM_BUILD_ROOT%{_docdir}/boost-jam
install -m 755 bin.linuxx86/jam $RPM_BUILD_ROOT%{_bindir}/jam
ln -sf jam $RPM_BUILD_ROOT%{_bindir}/bjam
install -m 644 Jam.html Jambase.html Jamfile.html README RELNOTES INSTALL \
        $RPM_BUILD_ROOT%{_docdir}/boost-jam

find $RPM_BUILD_ROOT -name CVS -type d -depth -exec rm -r {} \;

%files
%defattr(-,root,root)
%attr(755,root,root) /usr/bin/*
%doc %{_docdir}/boost-jam


%clean
rm -rf $RPM_BUILD_ROOT

