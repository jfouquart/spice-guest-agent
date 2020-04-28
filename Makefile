# Created by: Jason Helfman <jgh@FreeBSD.org>
# $FreeBSD: head/devel/spice-protocol/Makefile 490302 2019-01-14 16:41:59Z novel $

PORTNAME=	spice-agent
PORTVERSION=	0.18.0
DISTVERSIONPREFIX=	
CATEGORIES=	emulators

MAINTAINER=	jfouquart@hotmail.fr
COMMENT=	Spice agent for FreeBSD

LICENSE=	GPLv3

BUILD_DEPENDS=	${LOCALBASE}/include/alsa/asoundlib.h:audio/alsa-lib \
		gtk3>=3.12:x11-toolkits/gtk30 \
		spice-protocol>=0.12.15:devel/spice-protocol
LIB_DEPENDS=	libdbus-1.so:devel/dbus
RUN_DEPENDS=	alsa-lib>=1.1.1_1:audio/alsa-lib \
		gtk3>=3.12:x11-toolkits/gtk30 \
		spice-protocol>=0.12.15:devel/spice-protocol
		

GNU_CONFIGURE=	yes
USES=		autoreconf gmake pathfix pkgconfig xorg
USE_XORG=	x11

USE_GITLAB=     yes
GL_SITE=	https://gitlab.freedesktop.org
GL_ACCOUNT=	spice/linux
GL_PROJECT=	vd_agent
GL_COMMIT=	b190b1790219accae24166a36165ce66e8e02fd3

USE_RC_SUBR=	spice-agentd

post-patch:
	@${REINPLACE_CMD} 's|Exec=.*|Exec=${SH} -c "${PREFIX}/bin/spice-vdagent -x"|' ${WRKSRC}/data/spice-vdagent.desktop

.include <bsd.port.mk>
