from typing import Union, List
from matrix import Matrix

__version__ = "0.1.3"


def inject(data: Union[float, int, List, List[List]]) -> List[List[float]]:
    """
    Injects the input data into a matrix.

    Args:
        data (Union[float, int, List, List[List]]): The data to be injected into the Matrix.
            If data is a float or int, a 1x1 Matrix with the data value is returned.
            If data is a list of lists, it is assumed to be a Matrix and returned as is.
            If data is a list, it is treated as a single row Matrix.

    Returns:
        List[List[float]]: The injected matrix.

    Raises:
        TypeError: If the input data is not of the expected types.
    """
    if isinstance(data, float) or isinstance(data, int):
        matrix = [[float(data)]]
        return matrix
    elif isinstance(data, list):
        if all(isinstance(item, list) for item in data):
            return data 
        else: 
            matrix = [data]
            return matrix
    else: 
        raise TypeError("Please verify the input data.")


__all__ = [
    "Matrix",
    "inject",
]
