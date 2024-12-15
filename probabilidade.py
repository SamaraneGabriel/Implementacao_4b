import numpy as np
from PIL import Image


def calcular_t_link_capacidade(pixel_x, pixel_y, imagem_path, foreground_coords, background_coords):
    """
    Calcula as capacidades das arestas t-link de um pixel em uma imagem em escala de cinza.

    Args:
        pixel_x (int): Coordenada X do pixel na imagem.
        pixel_y (int): Coordenada Y do pixel na imagem.
        imagem_path (str): Caminho para a imagem no formato PPM.
        foreground_coords (tuple): Coordenadas da área de foreground no formato (x1, y1, x2, y2).
        background_coords (tuple): Coordenadas da área de background no formato (x1, y1, x2, y2).

    Returns:
        tuple: Capacidades do t-link para o nó Source e Sink (source_capacity, sink_capacity).
    """
    # Abrir a imagem PPM e converter para escala de cinza
    imagem = Image.open(imagem_path).convert("L")
    imagem_array = np.array(imagem)

    # Definir as áreas de foreground e background
    x1_f, y1_f, x2_f, y2_f = foreground_coords
    x1_b, y1_b, x2_b, y2_b = background_coords

    # Extrair os valores de intensidade das áreas
    foreground_area = imagem_array[y1_f:y2_f, x1_f:x2_f]
    background_area = imagem_array[y1_b:y2_b, x1_b:x2_b]

    # Calcular as médias de intensidade para foreground e background
    Ifmean = np.mean(foreground_area)
    Ibmean = np.mean(background_area)

    # Obter a intensidade do pixel em questão
    pixel_intensidade = imagem_array[pixel_y, pixel_x]

    # Calcular as capacidades do t-link usando as fórmulas de probabilidade
    source_capacity = -np.log(abs(pixel_intensidade - Ifmean) / (
        abs(pixel_intensidade - Ifmean) + abs(pixel_intensidade - Ibmean)))
    sink_capacity = -np.log(abs(pixel_intensidade - Ibmean) /
                            (abs(pixel_intensidade - Ifmean) + abs(pixel_intensidade - Ibmean)))

    return source_capacity, sink_capacity


def converter_para_cinza(imagem_path, output_path):
    """
    Converte uma imagem RGB para escala de cinza e salva o resultado.

    Args:
        imagem_path (str): Caminho da imagem original.
        output_path (str): Caminho para salvar a imagem em escala de cinza.
    """
    # Abrir a imagem e converter para escala de cinza
    imagem = Image.open(imagem_path).convert("L")
    imagem.save(output_path)
    print(f"Imagem convertida para escala de cinza salva em: {output_path}")


if __name__ == "__main__":
    # Caminho da imagem PPM
    imagem_path = "outputt.ppm"
    gray_image_path = "input_gray.ppm"

    # Converter para escala de cinza
    converter_para_cinza(imagem_path, gray_image_path)

    # Coordenadas escolhidas pelo usuário para foreground e background
    foreground_coords = (225, 225, 785, 291)  # (x1, y1, x2, y2)
    background_coords = (0, 1, 990, 160)  # (x1, y1, x2, y2)

    # Coordenadas do pixel para calcular o t-link
    pixel_x, pixel_y = 745, 735

    # Calcular capacidades na imagem em escala de cinza
    source_capacity, sink_capacity = calcular_t_link_capacidade(
        pixel_x, pixel_y, gray_image_path, foreground_coords, background_coords
    )

    print(f"Capacidade Source: {source_capacity}")
    print(f"Capacidade Sink: {sink_capacity}")
