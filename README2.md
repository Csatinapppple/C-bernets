# kubernetes parte 2
use este comando enquanto nessa pasta

curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64

```
sudo install minikube-linux-amd64 /usr/local/bin/minikube
```
para instalar o minikube para poder recriar esta atividade

comandos para minikube

Manage your cluster
Pause Kubernetes without impacting deployed applications:

minikube pause
Unpause a paused instance:

minikube unpause
Halt the cluster:

minikube stop
Change the default memory limit (requires a restart):

minikube config set memory 9001
Browse the catalog of easily installed Kubernetes services:

minikube addons list
Create a second cluster running an older Kubernetes release:

minikube start -p aged --kubernetes-version=v1.16.1
Delete all of the minikube clusters:

minikube delete --all
