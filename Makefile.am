ACLOCAL_AMFLAGS = -I m4

sbin_PROGRAMS = mstpd mstpctl

mstpd_SOURCES = \
	main.c epoll_loop.c brmon.c bridge_track.c libnetlink.c mstp.c \
	packet.c netif_utils.c ctl_socket_server.c hmac_md5.c driver_deps.c \
	platform/linux-bridge.c
mstpctl_SOURCES = \
	ctl_main.c ctl_socket_client.c

mstpd_CFLAGS = \
	-Os -Wall -D_REENTRANT -D__LINUX__ -I. \
	-D_GNU_SOURCE
if ENABLE_DEVEL
  mstpd_CFLAGS += -g3 -O0 -Werror
endif
mstpctl_CFLAGS = $(mstpd_CFLAGS)

all-local: bridge-stp utils/ifupdown.sh utils/mstp_config_bridge utils/mstpd.service

utilsdir=$(libexecdir)/mstpctl-utils

mstpdfile=$(sbindir)/mstpd
mstpctlfile=$(sbindir)/mstpctl
mstpdpidfile=$(localstatedir)/run/mstpd.pid
bridgestpconffile=$(sysconfdir)/bridge-stp.conf
ifupdownfile=$(utilsdir)/ifupdown.sh
utilsfuncfile=$(utilsdir)/mstpctl-utils-functions.sh
configbridgefile=$(utilsdir)/mstp_config_bridge
ifqueryfile=$(utilsdir)/ifquery

mstpd_CFLAGS += -DMSTPD_PID_FILE='"$(mstpdpidfile)"'

# See https://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.69/html_node/Installation-Directory-Variables.html#index-sysconfdir-188
populate_template = sed \
	-e 's|@mstpdfile[@]|$(mstpdfile)|g' \
	-e 's|@mstpdpidfile[@]|$(mstpdpidfile)|g' \
	-e 's|@mstpctlfile[@]|$(mstpctlfile)|g' \
	-e 's|@bridgestpconffile[@]|$(bridgestpconffile)|g' \
	-e 's|@ifupdownfile[@]|$(ifupdownfile)|g' \
	-e 's|@utilsfuncfile[@]|$(utilsfuncfile)|g' \
	-e 's|@configbridgefile[@]|$(configbridgefile)|g' \
	-e 's|@ifqueryfile[@]|$(ifqueryfile)|g'
bridge-stp utils/ifupdown.sh utils/mstp_config_bridge utils/mstpd.service: Makefile
	rm -f $@ $@.tmp
	mkdir -p $(@D)
	srcdir=''; test -f ./$@.in || srcdir="$(srcdir)/"; \
	$(populate_template) $${srcdir}$@.in > $@.tmp
	chmod 755 $@.tmp
	mv $@.tmp $@
bridge-stp: $(srcdir)/bridge-stp.in
utils/ifupdown.sh: $(srcdir)/utils/ifupdown.sh.in
utils/mstp_config_bridge: $(srcdir)/utils/mstp_config_bridge.in

sbindest=$(DESTDIR)$(sbindir)
utilsdest=$(DESTDIR)$(utilsdir)
etcdest=$(DESTDIR)$(sysconfdir)
bashcompdest=$(DESTDIR)$(bashcompletiondir)
systemdunitdest=$(DESTDIR)$(systemdunitdir)
man5dest=$(DESTDIR)$(mandir)/man5
man8dest=$(DESTDIR)$(mandir)/man8
docdest=$(DESTDIR)$(docdir)

install-data-hook:
	mkdir -pv $(sbindest)
	install -m 755 bridge-stp $(sbindest)/bridge-stp
	ln -sf $(sbindir)/bridge-stp $(sbindest)/mstp_restart
	mkdir -pv $(etcdest)
	install -m 644 $(srcdir)/bridge-stp.conf $(etcdest)/bridge-stp.conf
	mkdir -pv $(utilsdest)
	install -m 755 $(top_builddir)/utils/ifupdown.sh $(utilsdest)
	install -m 644 $(srcdir)/utils/mstpctl-utils-functions.sh $(utilsdest)
	ln -sf $(sbindir)/bridge-stp $(utilsdest)/mstpctl_restart_config
	install -m 755 $(top_builddir)/utils/mstp_config_bridge $(utilsdest)
	install -m 755 $(srcdir)/utils/ifquery $(utilsdest)
	if [ -d $(sysconfdir)/network/if-pre-up.d ]; then \
	  mkdir -pv $(etcdest)/network/if-pre-up.d ; \
	  ln -sf $(utilsdir)/ifupdown.sh $(etcdest)/network/if-pre-up.d/mstpctl ; \
	fi
	if [ -d $(sysconfdir)/network/if-post-down.d ]; then \
	  mkdir -pv $(etcdest)/network/if-post-down.d ; \
	  ln -sf $(utilsdir)/ifupdown.sh $(etcdest)/network/if-post-down.d/mstpctl ; \
	fi
	mkdir -pv $(bashcompdest)
	install -m 644 $(srcdir)/utils/bash_completion $(bashcompdest)/mstpctl
	if [ -n "$(systemdunitdir)" ]; then \
	  mkdir -pv $(systemdunitdest) ; \
	  install -m 644 $(top_builddir)/utils/mstpd.service $(systemdunitdest) ; \
	fi
	mkdir -pv $(man8dest)
	install -m 644 $(srcdir)/utils/mstpctl.8 $(man8dest)/mstpctl.8
	mkdir -pv $(man5dest)
	install -m 644 $(srcdir)/utils/mstpctl-utils-interfaces.5 $(man5dest)/mstpctl-utils-interfaces.5
	mkdir -pv $(docdest)
	install -m 644 $(srcdir)/README.VLANs.md $(docdest)/README.VLANs

rpm: rpm.spec
	Version="$$(perl -n -e 'if(/AC_INIT[^,]+,\s+\[([^]]+)\]/) \
	{ $$V=$$1; $$V=~s/-/_/g; print $$V; exit; }' configure.ac)" ; \
	[ -n "$$Version" ] || exit 3 ; SrcDir="$$(pwd)" ; \
	mkdir -p "../rpmbuild/SOURCES" ; cd "../rpmbuild/SOURCES" ; \
	git clone "$$SrcDir" "mstpd-$$Version" ; \
	tar -cvjf "mstpd-$$Version.tar.bz2" "mstpd-$$Version" ; \
	rm -rf "mstpd-$$Version" ; \
	cd .. ; mkdir -p SPECS ; cp "$$SrcDir/rpm.spec" SPECS/mstpd.spec
	rpmbuild --define "_topdir $$(pwd)" -ba SPECS/mstpd.spec

rpm.spec: configure.ac
	Version="$$(perl -n -e 'if(/AC_INIT[^,]+,\s+\[([^]]+)\]/) \
	{ $$V=$$1; $$V=~s/-/_/g; print $$V; exit; }' configure.ac)" ; \
	[ -n "$$Version" ] || exit 3 ; \
	perl -i -p -e 's/^(\s*Version:\s+).*$$/$${1}'"$$Version/" rpm.spec
