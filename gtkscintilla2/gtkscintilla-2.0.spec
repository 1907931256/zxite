%define name gtkscintilla-2.0
%define version 0.0.6
%define release paw.1
%define prefix  /usr

Summary:   GtkScintilla2 - A GTK+2 wrapper for Scintilla editing component
Name:      %{name}
Version:   %{version}
Release:   %{release}
Copyright: LGPL
Group:     System Environment/Libraries
Vendor:    PAW Digital Dynamics
Packager:  Dennis J Houy <djhouy@paw.co.za>
Url:       http://www.paw.co.za/projects/GtkScintilla2/
Source:    http://prdownloads.sourceforge.net/moleskine/%{name}-%{version}.tar.gz
Prefix:    %{prefix}
BuildRoot: %{_tmppath}/%{name}-root

%description
This is GtkScintilla2, a GTK+2 wrapper for the Scintilla (www.scintilla.org)
editing component wich adds some facility to this powerful component
written by Neil Hodgson (and may others).

%package devel
Summary:  GtkScintilla2 - Library header files and static libraries
Group:    Development/Libraries
Requires: %{name} = %{version}

%description devel
Headers files and static libraries needed for compiling other programs against
gtkscintilla2.

%prep
%setup -q

%build
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=${RPM_BUILD_ROOT} install-strip

%clean
rm -rf $RPM_BUILD_ROOT

%post
/sbin/ldconfig

%preun
/sbin/ldconfig

%files
%defattr(-, root, root)
%doc AUTHORS ChangeLog COPYING INSTALL NEWS README
%{_libdir}/*.so*

%files devel
%defattr(-, root, root)
%{_libdir}/pkgconfig/gtkscintilla-2.0.pc
%{_includedir}/gtkscintilla-2.0
%{_libdir}/*.a
%{_libdir}/*.la

%changelog
* Sat Mar 29 2003 Dennis J Houy <djhouy@paw.za.org>
- Updated to package with changes from Sven Herzberg

* Tue Jan 30 2003 Andy Jeffries <andy@gphpedit.org>
- Updated to use Scintilla 1.50 (from 1.48), added fix from 
  Neil Hodgson to use Scroll wheel under GTK+2.0

* Tue Jan 14 2003 Dennis J Houy <djhouy@paw.co.za>
- Implemented gtk_scintilla_get_text_range

* Tue Jan 07 2003 Dennis J Houy <djhouy@paw.co.za>
- License changed to LGPL.

* Thu Oct 10 2002 Dennis J Houy <djhouy@paw.co.za>
- Initial build.

