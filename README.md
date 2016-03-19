O diretório lib contém três sessões cg, ui, utils. Dentro de cg estão
componentes de modelagem de dados, estrutura de dados, componentes tais como
classes de objetos a serem desenhados. UI significa user interface e segurará as
classes responsáveis pela gestão da interface gráfica em GTKMM que é um wrapper
para C++, do GTK. O diretório utils é mais flexível e contém programas que
atribuem algo à UI ou CG que servem como utilitários para desempenhar algum
serviço ou macro que não se encaixa na camada de modelo.

Para compilar esse projeto basta você baixar ele. E na sua máquina LINUX, com
distribuição Ubuntu e derivados, você deverá se certificar que existem libs
instaladas para compilar com o gcc.

Crie os projetos com seu eclipse, ou crie Makefiles para os projetos dentro
desse repositório. Talvez você precise instalar alguma lib a mais para threads
ou relacionadas.
