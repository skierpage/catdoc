Name: catdoc
Version: v0.97_devel
Release: %autorelease
Summary: programs which extract text from Microsoft Office 97-2004 files
License: GPL-2.0-or-later
URL: https://github.com/skierpage/catdoc
Source0: https://github.com/skierpage/%{name}/archive/refs/tags/%{version}.tar.gz
BuildRequires: gcc
BuildRequires: make
BuildRequires: tk
Recommends: %{name}-wordview

%description
catdoc is a program which reads MS-Office 1997-2004 Word .doc files and prints
their content as readable text to stdout. It supports multiple character sets
for input and output.

The catdoc package also includes
- `catppt`, which reads MS PowerPoint `.ppt` files and prints their
  content.
- `xls2csv`, which reads MS Excel `.xls` files and prints their content as
  rows of comma-separated values.
- `wordview`, which displays `catdoc` output in a window.

%package wordview
Summary: Display Microsoft Word .doc files in a graphical window
Requires: catdoc
Requires: tk
%if 0%{?fedora} > 41
Conflicts: %{name} <= 0.95-26
%endif
%if 0%{?fedora} == 41
Conflicts: %{name} <= 0.95-26
%endif

%description wordview
wordview is a program that displays Microsoft Word files in a graphical window
via Tk.

%prep
%autosetup -p1 -n catdoc-v0.96

%build
%configure
%make_build

%install
%make_install

# Struggling to get this to work
#       %check
#       %{make} check
# fails when I invoke `packit build locally` with
#        /var/tmp/rpm-tmp.5GB8PL: line 47: fg: no job control
#        error: Bad exit status from /var/tmp/rpm-tmp.5GB8PL (%check)
#       %{__make} check
# works, but fails because it runs /usr/bin/catdoc, not the just-built BUILDROOT/usr/bin/
# %check
# %{__make} check

%files
%license COPYING
%{_bindir}/catdoc
%{_bindir}/catppt
%{_bindir}/xls2csv
%{_mandir}/man1/catdoc.1.*
%{_mandir}/man1/catppt.1.*
%{_mandir}/man1/xls2csv.1.*
%{_datadir}/catdoc
%doc README.md NEWS

%files wordview
%{_bindir}/wordview
%{_mandir}/man1/wordview.1.*

%changelog
%autochangelog
