Instrucciones para compilar y hacer hook para la función de generar huevos pokémon personalizados.

Aviso! Si repunteaste la tabla gEvolutionTable en 0x259754, debes abrir el archivo BPRE.ld y actualizar el offset correspondiente.

Luego de modificar el código de acuerdo a las necesidades de su hack, deben seguir las siguientes instrucciones para compilar el código e inyectarlo en su ROM.

Requisitos:
	-DevkitARM
	-ARMips
	-Make 
	-*Cualquier terminal que permita compilar proyectos de decompilación sirve (Cygwin,MSys2,WSL)

-1-. Hacer copia de seguridad de tu rom y luego pon una copia de tu rom renombrado "BPRE0.gba" en la carpeta principal.

-2-. Modificar la línea ".org 0x08FC0000" en "main.s", con una dirección con espacio libre en tu rom. Es posible que también sea necesario modificar la línea "ORIGIN = 0x08FC0000" en "linker.ld".

-3-. Abren la terminal que usarán para compilar, en mi caso yo uso Cygwin64.

-4-. Escribimos cd seguido de la ruta donde tenemos copia del repositorio, y damos enter.

-5-. Escribimos make y damos enter.

-6-. Esperamos unos segundos a que termine. Si aparecen errores, luego explicaré cómo solucionarlos.

-7-. Luego de ejecutar make, una nueva rom llamada "rom.gba" aparecerá en la carpeta "build", la abrimos con HxD o con cualquier editor hexadecimal.

-8-. Abrimos el archivo offsets.txt que se generó luego de ejecutar make, en el paso 5. De ahí tomaremos 2 offset.

-9-. [Hook] El primer offset será el de hookaddhatchedmontoparty, copiamos lo que va después del 08, en mi caso 0xFC0404, nos dirigimos a ese offset en el editor hexadecimal, copiamos 0xC0 bytes y lo pegamos en el offset 0x46d60

10-. El segundo offset será el de la función "customgiveegg" que en mi caso es 08FC0000, copiamos lo que va después del 08, esto en los scripts será llamado como "callasm 0xFC0001".

11-. [Script] Ahora podemos usar la función para generar huevos personalizados. El siguiente ejemplo es un script de prueba para el uso del código sin modificaciones extra.

'---------------Script de prueba
#dynamic 0x800000

#org @icinio
lock
faceplayer
setvar 0x8000 x 'Especie, si es 0 obtiene una especie random sin evolucionar.
setvar 0x8001 x 'nivel, del 1 al 100
setvar 0x8002 x 'ball del 1 al 12, predeterminado es 4 = pokeball
setvar 0x8003 x 'ciclos es igual a (n + 1) * 256 pasos para eclosionar. máximo 255 ciclos.
setvar 0x8004 x 'IVs, de 0 a 31 todos iguales, 32 = aleatorio
callasm 0xFC0001 'Offset+1 de la función "customgiveegg"
msgbox @texto MSG_KEEPOPEN
release
end

#org @texto
= Toma un huevo.

