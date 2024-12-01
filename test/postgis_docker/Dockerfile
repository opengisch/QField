FROM kartoza/postgis:16-3.4

COPY ./cert_keys/root.crt /var/lib/postgresql/root.crt
COPY ./cert_keys/server.crt /var/lib/postgresql/server.crt
COPY ./cert_keys/server.key /var/lib/postgresql/server.key

RUN chown postgres /var/lib/postgresql/root.crt && chmod 700 /var/lib/postgresql/root.crt
RUN chown postgres /var/lib/postgresql/server.crt && chmod 700 /var/lib/postgresql/server.crt
RUN chown postgres /var/lib/postgresql/server.key && chmod 700 /var/lib/postgresql/server.key
