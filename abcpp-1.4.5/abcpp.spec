Name:      abcpp
Version:   1.4.0
Release:   1
Summary:   abcpp: an ABC file preprocessor
License:   GPL
URL:       http://abcplus.sourceforge.net
Group:     Applications/File
Prefix:    %{_prefix}
BuildRoot: %{_tmppath}/%{name}-%{version}-buildroot
Source:    http://abcplus.sourceforge.net/%{name}-%{version}.tar.gz

%description
abcpp is a simple yet powerful preprocessor designed for, but not limited
to, ABC music files. It allows the user to write portable ABC files, change
the syntax, extract parts, etc.

%prep
%setup -q

%build
make CFLAGS="$RPM_OPT_FLAGS -Wall"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
mkdir -p $RPM_BUILD_ROOT%{_datadir}/abcpp
%{_bindir}/install -s -m 755 abcpp $RPM_BUILD_ROOT%{_bindir}
%{_bindir}/install -m 644 fancyheader.abp $RPM_BUILD_ROOT%{_datadir}/abcpp
%{_bindir}/install -m 644 italiano.abp $RPM_BUILD_ROOT%{_datadir}/abcpp

%clean
rm -rf "$RPM_BUILD_ROOT"

%files
%defattr(-,root,root)
%doc COPYING INSTALL LEGGIMI README docs
%doc examples
%{_bindir}/*
%{_datadir}/abcpp

%changelog
* Wed Jun 04 2003 Guido Gonzato
- updated to 1.2.4

* Wed Oct 30 2002 Guido Gonzato
- updated to 1.2.2

* Mon Dec 31 2001 José Romildo Malaquias <romildo@iceb.ufop.br> 1.1.1-2
- More use of macros in spec file
- Use $RPM_OPT_FLAGS at compilation
- A small fix in the %files section
- Include the examples in the documentation

