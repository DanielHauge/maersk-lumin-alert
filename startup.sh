#!/bin/bash

docker run -d --rm -p 9090:9090 --name prom --network bridge prom/prometheus
docker run -d --rm -p 3000:3000 --network bridge --name=grafana grafana/grafana-enterprise

# Add sensor unit
docker exec prom sh -c "sed -i '29s/9090\"/9090\",\"192.168.0.158:8080\"/' /etc/prometheus/prometheus.yml"
# Increase flow rate and eval rate
docker exec prom sh -c "sed -i '3s/15s/1s/' /etc/prometheus/prometheus.yml"
docker exec prom sh -c "sed -i '4s/15s/1s/' /etc/prometheus/prometheus.yml"
# Restart for init
docker restart prom
