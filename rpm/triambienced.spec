#
# triambienced RPM spec
#
# (C) kimmoli 2016
# (C) Jami Kettunen 2019
#

Name:       triambienced

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    Tristate ambience switcher daemon
Version:    1.0.0
Release:    1
Group:      System/Daemons
License:    LICENSE
URL:        https://github.com/sailfishos-oneplus5/triambience-daemon
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(mlite5)

%description
Control currently active ambiences using the tristate "notification" slider on your device.

%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5 SPECVERSION=%{version}

%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}

%qmake5_install

%preun
# in case of complete removal, stop and disable
if [ "$1" = "0" ]; then
  systemctl-user stop triambienced || true
  systemctl-user disable triambienced || true
fi

%post
systemctl-user daemon-reload || true
systemctl-user start triambienced || true
systemctl-user enable triambienced || true

%pre
# In case of update, stop first
if [ "$1" = "2" ]; then
  systemctl-user stop triambienced || true
  systemctl-user disable triambienced || true
fi

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/%{name}
/usr/lib/systemd/user/
