// #include <pybind11/pybind11.h>
#include <torch/extension.h>

#include <pybind11/numpy.h>
#include <pybind11/eigen.h>

#include <gpu/api_gpu.h>
#include <cpu/api_cpu.h>


namespace py = pybind11;
using namespace cubvh;

PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {

// CUDA API
py::class_<cuBVH>(m, "cuBVH")
    .def("ray_trace", &cuBVH::ray_trace)
    .def("unsigned_distance", &cuBVH::unsigned_distance)
    .def("signed_distance", &cuBVH::signed_distance);

m.def("create_cuBVH", &create_cuBVH);

m.def("floodfill", &floodfill);

m.def("sparse_marching_cubes", &sparse_marching_cubes, 
    py::arg("coords"), py::arg("corners"), 
    py::arg("iso"), 
    py::arg("ensure_consistency") = false
);

// GPU Hash Table bindings
py::class_<cuHashTable>(m, "cuHashTable")
    .def("set_num_dims", &cuHashTable::set_num_dims)
    .def("get_num_dims", &cuHashTable::get_num_dims)
    .def("resize", &cuHashTable::resize)
    .def("prepare", &cuHashTable::prepare)
    .def("insert", &cuHashTable::insert, py::arg("coords"))
    .def("build", &cuHashTable::build, py::arg("coords"))
    .def("search", &cuHashTable::search, py::arg("queries"));

m.def("create_cuHashTable", &create_cuHashTable);

// CPU API
m.def("fill_holes", &fill_holes,
    py::arg("vertices"), py::arg("faces"),
    py::arg("return_added") = false,
    py::arg("check_containment") = true,
    py::arg("eps") = 1e-6,
    py::arg("verbose") = false
);

m.def("merge_vertices", &merge_vertices, 
    py::arg("vertices"), py::arg("faces"), 
    py::arg("threshold")
);

m.def("sparse_marching_cubes_cpu", &sparse_marching_cubes_cpu,
    py::arg("coords"), py::arg("corners"), 
    py::arg("iso"), 
    py::arg("ensure_consistency") = false
);

// CPU Hash Table bindings
py::class_<HashTable>(m, "HashTable")
    .def(py::init<>())
    .def("set_num_dims", &HashTable::set_num_dims)
    .def("get_num_dims", &HashTable::get_num_dims)
    .def("resize", &HashTable::resize)
    .def("prepare", &HashTable::prepare)
    .def("insert", &HashTable::insert, py::arg("coords"))
    .def("build", &HashTable::build, py::arg("coords"))
    .def("search", &HashTable::search, py::arg("queries"));
}