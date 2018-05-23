# ANE-STENT
Ane-stent permite a sus usuarios crear visualizaciones de interacción física entre modelos
en formatos VTP y VTK, combinando cuerpos rígidos y suaves con comportamiento
específico para cada modelo. Utilizando Bullet Physics y VTK

## Abstract

A proof of concept was developed, to evaluate the feasibility of developing a physics
simulation of rigid and soft bodies, integrating Bullet Physics with VTK. Building upon
the research of the Thrombus-VPH project.

## Resumen

Se realizó una prueba de concepto, evaluando la factibilidad del desarrollo de una
simulación de física de cuerpos rígidos y suaves, integrando Bullet Physics con VTK.
Continuando así con la investigación del proyecto Thrombus-VPH.


## Introducción

Los stents son cilindros con paredes de malla, que se usan en tratamientos mínima-mente invasivos para varias enfermedades cardiovasculares.
Existe una amplia variedad de stents en el mercado e incertidumbre sobre la in-fluencia en el proceso de tratamiento de cada variación. Esto dificulta la selección del stent óptimo para cada caso médico específico.
Ane-stent es una prueba de concepto de una simulación física hecha en C++ que integra dos librerías de código abierto:

* Bullet Physics, una librería de física que permite simular cuerpos rígidos y suaves, con restricciones y en tiempo real.
* VTK, una librería de visualización de datos usada frecuentemente en aplica-ciones que trabajan con imágenes médicas.

