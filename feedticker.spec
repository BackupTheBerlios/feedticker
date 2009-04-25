Summary: 
Name: 
Version: 
Release: 
Epoch: 0
License: GPL
Group: 
Source0: 
Source1: 
Patch0: 
Patch1: 
URL: 
BuildRoot: %{_tmppath}/%{name}-root
Requires: /sbin/ldconfig

%description

%prep
%setup -q

%build
%configure
make

%install
rm -rf %{buildroot}
%makeinstall


%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog NEWS README TODO
%{_bindir}/*
%{_libdir}/*.so.*
%{_datadir}/%{name}
%{_mandir}/man8/*

%changelog
* Mon Apr 28 2003 FirstName LastName <email@address.com>
- Initial spec file
