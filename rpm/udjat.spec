#
# spec file for package udjat
#
# Copyright (c) 2015 SUSE LINUX GmbH, Nuernberg, Germany.
# Copyright (C) <2008> <Banco do Brasil S.A.>
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

%define product_name %(pkg-config --variable=product_name libudjat)
%define module_path %(pkg-config --variable=module_path libudjat)

Name: udjat
Summary: Udjat main service

Version: 0.0.0
Release: 0

License: LGPL-3.0
Source: %{name}-%{version}.tar.xz
URL: https://github.com/PerryWerneck/udjat
Group: System/Daemons

BuildRoot: /var/tmp/%{name}-%{version}

# Development core
BuildRequires:	coreutils
BuildRequires:	sed
BuildRequires:	automake
BuildRequires:	autoconf

# Bibliotecas & Ferramentas
BuildRequires:  gcc-c++ >= 4.8
BuildRequires:	fdupes
BuildRequires:	libtool

BuildRequires:	pkgconfig(systemd)
BuildRequires:	pkgconfig(libsystemd)
BuildRequires:	pkgconfig(libudjat)

%systemd_requires

PreReq:			coreutils
PreReq:			permissions

%description
Main service for udjat, requires a configuration and modules to work.

#---------------------------------------------------------------------------------

%prep

%setup
NOCONFIGURE=1 ./autogen.sh
%configure

%build
make clean
make Release

%install
rm -rf $RPM_BUILD_ROOT

%make_install

# Remove duplicatas
%fdupes $RPM_BUILD_ROOT

%if 0%{?suse_version} >= 1500

# https://packageninjas.github.io/packaging/2020/10/13/news-in-packaging.html#file-triggers
%transfiletrigger -- /etc/%{name}.conf.d
echo "Restarting %{name}"
%{_sbindir}/rc%{name} restart

%transfiletrigger -- /etc/%{name}.xml.d
echo "Restarting %{name}"
%{_sbindir}/rc%{name} restart

%endif

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

%{_sbindir}/%{name}
%{_sbindir}/rc%{name}

%dir %attr(755,root,wheel) %{_datadir}/%{name}
%dir %attr(755,root,wheel) /etc/%{name}.conf.d/
%dir %attr(755,root,wheel) /etc/%{name}.xml.d/

%{_unitdir}/%{name}.service
%{_presetdir}/50-%{name}.preset

%config(noreplace) /etc/%{name}.conf.d/*.conf

%exclude %{_sysconfdir}/permissions.d/*

%pre
%service_add_pre %{name}.service

%post
%service_add_post %{name}.service

%preun
%service_del_preun %{name}.service

%postun
%service_del_postun %{name}.service

%pretrans
if [ -x /usr/sbin/rc%{name} ]; then
	echo "Stopping %{name}"
	/usr/sbin/rc%{name} stop &
fi

%posttrans
# Só reinicio no final da transação.
if [ -x /usr/sbin/rc%{name} ]; then
	echo "Restarting %{name}"
	/usr/sbin/rc%{name} restart &
fi

%changelog


