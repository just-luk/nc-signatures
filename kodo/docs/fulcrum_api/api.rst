===========
Fulcrum API
===========

Overview of the public fulcrum code API.

The Fulcrum network codes use a concatenated code structure with an "outer"
and "inner" code.
They combine the performance benefits of using GF(2) while providing a low
degree of linear dependency as expected from larger finite fields.

For a detailed description of the Fulcrum codec, see the following paper 
"`Fulcrum Network Codes: A Code for Fluid Allocation of Complexity <http://arxiv.org/abs/1404.6620>`_" by Lucani et. al.


.. toctree::
   :maxdepth: 2

   fulcrum_encoder
   fulcrum_decoder
   fulcrum_generator_random_uniform
