# Created by: Jason Helfman <jgh@FreeBSD.org>
# $FreeBSD: head/devel/spice-protocol/Makefile 490302 2019-01-14 16:41:59Z novel $

PORTNAME=	spice-agent
PORTVERSION=	0.21.0
CATEGORIES=	emulators

MAINTAINER=	jfouquart@hotmail.fr
COMMENT=	Spice agent for FreeBSD

LICENSE=	GPLv3

BUILD_DEPENDS=	${LOCALBASE}/include/alsa/asoundlib.h:audio/alsa-lib \
		gtk3>=3.12:x11-toolkits/gtk30 \
		spice-protocol>=0.12.15:devel/spice-protocol
LIB_DEPENDS=	libdbus-1.so:devel/dbus
RUN_DEPENDS=	alsa-lib>=1.1.1_1:audio/alsa-lib \
		alsa-utils>0:audio/alsa-utils \
		gtk3>=3.12:x11-toolkits/gtk30 \
		spice-protocol>=0.14.3:devel/spice-protocol

GNU_CONFIGURE=	yes
USES=		autoreconf gmake pathfix pkgconfig xorg
USE_XORG=	x11

USE_GITLAB=	yes
GL_SITE=	https://gitlab.freedesktop.org
GL_ACCOUNT=	spice/linux
GL_PROJECT=	vd_agent
GL_COMMIT=	9306f18621f53723087d355ba0e4f63a96dd87f1

SUB_FILES=	pkg-message

USE_RC_SUBR=	spice-agentd

.include <bsd.port.options.mk>

.if ${OSVERSION} >= 1300000
REPLACE_CMD=	${PREFIX}/bin/spice-vdagent
.else
REPLACE_CMD=	${SH} -c "${PREFIX}/bin/spice-vdagent -x \&"
.endif

post-patch:
	@${REINPLACE_CMD} -E 's|Exec=.*|Exec=${REPLACE_CMD}|' ${WRKSRC}/data/spice-vdagent.desktop

post-install:
	${MKDIR} ${STAGEDIR}${PREFIX}/etc/X11/xorg.conf.d
	${INSTALL_DATA} ${FILESDIR}/spice_vdagent_tablet.conf \
		${STAGEDIR}${PREFIX}/etc/X11/xorg.conf.d

.include <bsd.port.mk>
