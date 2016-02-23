#
# triambience spec
# (C) kimmoli 2016
#

Name:       triambience

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Summary:    Tristate ambience switcher
Version:    0.0.devel
Release:    1
Group:      Qt/Qt
License:    LICENSE
URL:        https://github.com/kimmoli/triambience
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(mlite5)

%description
%{summary}

%prep
%setup -q -n %{name}-%{version}

%build

%qtc_qmake5 SPECVERSION=%{version}

%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}

%qmake5_install

%preun
systemctl-user stop triambience
systemctl-user disable triambience

%post
systemctl-user start triambience
systemctl-user enable triambience
mkdir -p /home/nemo/.config/systemd/user/post-user-session.target.wants
ln -s '/etc/systemd/user/triambience.service' '/home/nemo/.config/systemd/user/post-user-session.target.wants/triambience.service'
chown -hR 100000:100000 /home/nemo
chmod 755 /home/nemo/.config/systemd
chmod 755 /home/nemo/.config/systemd/user
chmod 755 /home/nemo/.config/systemd/user/post-user-session.target.wants
chmod 777 /home/nemo/.config/systemd/user/post-user-session.target.wants/triambience.service


%pre
# In case of update, stop first
if [ "$1" = "2" ]; then
  systemctl-user stop triambience
fi

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/%{name}
/etc/systemd/user/

