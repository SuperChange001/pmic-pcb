# Elastic Power Module

This repository contains the design and related files for the Elastic Power Module.

![Elastic Power Module](pictures/elastic_power_module_side_view.jpg)

## Current Status
- [x] Schematic
- [x] PCB Design
- [x] Prototype
- [ ] Testing (** It is not tested yet, DO NOT use it as your reference design! **)
  - know issues:
    - in the default configuration, the nRST pin should be pulled up to VSYS, but right now it is floating.
      - should further check if I always need to have the external pull-up resistor or I can use the internal pull-up resistor of the microcontroller.
      - and how convinent it is to keep current circuit and configure the IC via microcontroller.
    - VIO should be connected to the header or a test pad, so we can configure the IC without flying a wire.

## Project Structure

- `elastic_power_module.kicad_pcb`: KiCad PCB design file.
- `elastic_power_module.kicad_prl`: KiCad project local settings file.
- `elastic_power_module.kicad_pro`: KiCad project file.
- `elastic_power_module.kicad_sch`: KiCad schematic file.
- `elastic_power_module.pdf`: PDF version of the schematic.
- `fp-info-cache`: Footprint info cache.
- `pictures`: Contains pictures of the assembled module.
  - `elastic_power_module_bottom_view.jpg`: Bottom view of the module.
  - `elastic_power_module_side_view.jpg`: Side view of the module.
  - `elastic_power_module_top_view.jpg`: Top view of the module.~

## Getting Started

- To get started with this project, clone the repository and open the `.kicad_pcb` or `.kicad_sch` files in KiCad.
- Or you can just download the PDF version of the schematic and use it as your reference design.
- To assemble the module, you can use the interactive BOM file [here](https://htmlpreview.github.io/?https://github.com/SuperChange001/pmic-pcb/blob/master/bom/ibom.html).

## Contributing

Contributions are welcome. Please open an issue to discuss your idea or submit a Pull Request.

## License

[MIT](https://choosealicense.com/licenses/mit/)~~~