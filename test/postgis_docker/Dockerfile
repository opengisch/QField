FROM kartoza/postgis:16-3.4

COPY ./cert_keys/fullchain.pem /var/lib/postgresql/root.crt
COPY ./cert_keys/fullchain.pem /var/lib/postgresql/fullchain.pem
COPY ./cert_keys/privkey.pem /var/lib/postgresql/privkey.pem

RUN chown postgres /var/lib/postgresql/root.crt && chmod 700 /var/lib/postgresql/root.crt
RUN chown postgres /var/lib/postgresql/fullchain.pem && chmod 700 /var/lib/postgresql/fullchain.pem
RUN chown postgres /var/lib/postgresql/privkey.pem && chmod 700 /var/lib/postgresql/privkey.pem
