#ifndef __DDL_CATALOGADOR_H_
#define __DDL_CATALOGADOR_H_

static const char* ddl = "\
create table dia ( \
	id_dia integer primary key autoincrement, \
	dia    date not null, \
\
	unique(dia) \
); \
create table directorio( \
	id_directorio integer primary key autoincrement, \
	nombre text not null collate nocase, \
	estado integer default 0, \
\
	unique(nombre) \
);\
create table archivo( \
	id_archivo integer primary key autoincrement, \
	nombre  text not null, \
	tamanio integer not null, \
	fecha   timestamp not null, \
	md5     text default 0, \
	sha1    text default 0, \
	sha512  text default 0, \
	estado  integer default 0, \
\
	UNIQUE(nombre, tamanio, fecha, md5, sha1, sha512) \
);\
create table etiqueta( \
	id_etiqueta integer primary key autoincrement, \
	nombre      text not null, \
	directorio  text not null collate nocase, \
	filtro      text, \
	excluir     text, \
	opciones    integer default 0, \
\
	UNIQUE(nombre) \
); \
create table catalogo( \
	id_catalogo		integer primary key autoincrement, \
	id_etiqueta		integer not null, \
	id_directorio	integer not null, \
	id_archivo		integer not null, \
	agregado		datetime default (datetime(CURRENT_TIMESTAMP,'localtime')), \
	estado 			integer default 0, \
\
	UNIQUE(id_etiqueta, id_directorio, id_archivo), \
	FOREIGN KEY(id_directorio) REFERENCES directorio(id_directorio) on delete cascade, \
	FOREIGN KEY(id_archivo) REFERENCES archivo(id_archivo) on delete cascade, \
	FOREIGN KEY(id_etiqueta) REFERENCES etiqueta(id_etiqueta) on delete cascade \
); \
create table catalogoV2(\
	id_catalogo		integer primary key autoincrement, \
	id_etiqueta		integer not null, \
	id_directorio	integer not null, \
	archivo			text not null collate nocase, \
	tamanio			integer not null, \
	fecha			timestamp not null, \
	md5				text default 0 collate nocase, \
	sha1			text default 0 collate nocase, \
	sha512			text default 0 collate nocase, \
	estado			integer default 0, \
	agregado		datetime default (datetime(CURRENT_TIMESTAMP, 'localtime')), \
	\
	FOREIGN KEY(id_directorio) REFERENCES directorio(id_directorio) on delete cascade, \
	FOREIGN KEY(id_etiqueta) REFERENCES etiqueta(id_etiqueta) on delete cascade, \
	\
	UNIQUE(id_directorio,archivo) \
); \
create index idx_archivo on catalogoV2 (archivo);\
";

#endif
