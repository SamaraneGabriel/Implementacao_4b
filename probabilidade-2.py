import sys
import numpy as np
import json
from PIL import Image
from sklearn.cluster import KMeans
from scipy.stats import norm
from tqdm import tqdm
from joblib import Parallel, delayed

def calcular_distribuicoes_gmm(imagem_flat, labels, centers):
    """
    Calcula médias e desvios padrão para foreground e background baseados nos clusters do K-Means.
    """
    foreground_pixels = imagem_flat[labels == np.argmax(centers)]
    background_pixels = imagem_flat[labels == np.argmin(centers)]

    # Ajustar distribuições Gaussianas
    fg_dist = norm(np.mean(foreground_pixels), np.std(foreground_pixels))
    bg_dist = norm(np.mean(background_pixels), np.std(background_pixels))

    return fg_dist, bg_dist

def calcular_t_link_capacidade_gaussiana(pixel_x, pixel_y, imagem_array, fg_dist, bg_dist):
    """
    Calcula as capacidades das arestas t-link de um pixel baseado nas distribuições Gaussianas.
    """
    # Obter a intensidade do pixel
    pixel_intensidade = imagem_array[pixel_y, pixel_x]

    # Calcular as probabilidades
    source_capacity = -np.log(fg_dist.pdf(pixel_intensidade) + 1e-10)  # Evitar log(0)
    sink_capacity = -np.log(bg_dist.pdf(pixel_intensidade) + 1e-10)

    return source_capacity, sink_capacity

def processar_imagem_e_calcular_capacidades_gaussianas(imagem_path, pixels, clusters=2):
    """
    Processa a imagem inteira e calcula as capacidades dos t-links para múltiplos pixels.
    """
    # Abrir a imagem e converter para escala de cinza
    imagem = Image.open(imagem_path).convert("L")
    imagem_array = np.array(imagem)

    # Aplicar K-Means clustering
    imagem_flat = imagem_array.flatten()
    kmeans = KMeans(n_clusters=clusters, random_state=42, n_init=10)
    labels = kmeans.fit_predict(imagem_flat.reshape(-1, 1))
    centers = kmeans.cluster_centers_

    # Obter distribuições Gaussianas
    fg_dist, bg_dist = calcular_distribuicoes_gmm(imagem_flat, labels, centers)

    # Processar cada pixel em paralelo
    resultados = Parallel(n_jobs=-1)(
        delayed(calcular_t_link_capacidade_gaussiana)(
            x, y, imagem_array, fg_dist, bg_dist
        ) for x, y in tqdm(pixels, desc="Calculando capacidades")
    )

    return resultados

def converter_para_cinza(imagem_path, output_path):
    """
    Converte uma imagem RGB para escala de cinza e salva o resultado.
    """
    imagem = Image.open(imagem_path).convert("L")
    imagem.save(output_path)

def main():
    if len(sys.argv) != 2:
        print("Usage: python probabilidade-batch.py <json_file>")
        sys.exit(1)

    # Receber o caminho do arquivo JSON
    json_file = sys.argv[1]

    try:
        with open(json_file, "r") as file:
            payload = json.load(file)
    except FileNotFoundError:
        print(f"Error: File {json_file} not found.")
        sys.exit(1)
    except json.JSONDecodeError as e:
        print(f"Error: File {json_file} contains invalid JSON: {e}")
        sys.exit(1)

    # Extrair os dados do JSON
    image_path = payload.get("image")
    pixels = payload.get("pixels", [])

    if not image_path or not pixels:
        print("Error: Invalid JSON structure. 'image' or 'pixels' is missing.")
        sys.exit(1)

    # Converter a imagem para escala de cinza
    cinza_path = "input_gray.ppm"
    converter_para_cinza(image_path, cinza_path)

    # Processar a imagem e calcular capacidades
    resultados = processar_imagem_e_calcular_capacidades_gaussianas(cinza_path, pixels)

    # Retornar os resultados em formato simples
    for result in resultados:
        print(f"{result[0]} {result[1]}")

if __name__ == "__main__":
    main()
