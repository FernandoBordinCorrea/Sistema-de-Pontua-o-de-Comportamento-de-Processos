#  Sistema de Pontuação de Comportamento de Processos

Este projeto implementa um **módulo do kernel Linux** projetado para monitorar continuamente o comportamento de processos em execução no sistema.  
A ideia é avaliar **métricas críticas de desempenho e uso de recursos**, atribuindo a cada processo uma **nota de risco** (`Baixo`, `Médio` ou `Alto`), de acordo com o consumo e o comportamento observado.

---

##  Funcionalidades

- Monitoramento contínuo das principais métricas de cada processo:
  - Uso de **CPU**
  - Número de **chamadas de sistema**
  - Atividade de **entrada/saída (E/S)**
- Algoritmo de avaliação de risco com base em limites e comportamento anômalo.
- Exposição das informações em um arquivo no **/proc**, permitindo inspeção com ferramentas como:
  ```bash
  cat /proc/risk_monitor
