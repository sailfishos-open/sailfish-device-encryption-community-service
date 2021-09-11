Name:       sailfish-device-encryption-community-service

Summary:    Service for Sailfish Device Encryption
Version:    0.1
Release:    1
Group:      Qt/Qt
License:    GPLv2
URL:        https://github.com/sailfishos-open/sailfish-device-encryption-community-service
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  libsfosdevenc-devel
BuildRequires:  desktop-file-utils
BuildRequires:  cmake
BuildRequires:  qt5-qttools-linguist

%description
%summary

%prep
%setup -q -n %{name}-%{version}

%build

%cmake . 
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%make_install

# systemd unit
mkdir -p %{buildroot}%{_unitdir}
install -t %{buildroot}%{_unitdir} --mode=644 systemd/sailfish-device-encryption-community-service.service

%post
systemctl restart sailfish-device-encryption-community-service.service || true

%postun
systemctl stop sailfish-device-encryption-community-service.service || true

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/dbus-1
%{_unitdir}
%{_datadir}/%{name}
